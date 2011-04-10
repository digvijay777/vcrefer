/*
 *	注册表监控
 */
#include "XRegmon.h"
#include <stdio.h>
#include <stdarg.h>

#define SYSNAME         "System"

HANDLE					ghEventMetux			= NULL;
HANDLE					ghEventBadopt			= NULL;
PSRVTABLE				ServiceTable			= NULL;
fnRealRegSetValueKey	RealRegSetValueKey		= NULL;
ULONG					ProcessNameOffset		= 0;

NTSTATUS RegSetValueKey( IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName,
						IN ULONG TitleIndex, IN ULONG Type, 
						IN PVOID Data, IN ULONG DataSize );


void EnableServiceTable()
{
	__asm{
		push eax 
		mov	 eax, cr0 
		and	 eax, 0fffeffffh 
		mov  cr0, eax 
		pop  eax

	}
}

void DisableServiceTable()
{
	__asm{ 
		push eax 
		mov  eax, cr0 
		or	 eax, not 0fffeffffh 
		mov  cr0, eax 
		pop  eax 
	} 
}

ULONG GetProcessNameOffset()
{
	PEPROCESS       curproc;
	int             i;

	curproc = PsGetCurrentProcess();

	//
	// Scan for 12KB, hopping the KPEB never grows that big!
	//
	for( i = 0; i < 3*PAGE_SIZE; i++ ) {

		if( !strncmp( SYSNAME, (PCHAR) curproc + i, strlen(SYSNAME) )) {

			return i;
		}
	}

	//
	// Name not found - oh, well
	//
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// 入口函数
// #pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject
								, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS		status = STATUS_SUCCESS;
	int				i = 0;

	LogWrite("Enter DriverEntry\r\n");
	// 注册其他驱动调用函数入口
	for(; i < arrayof(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = DDKXRegmonDispatchRoutine;
	}

	pDriverObject->DriverUnload = DDKXRegmonUnload;
	pDriverObject->MajorFunction[IRP_MJ_READ] = DDKXRegmonDispatchRead;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DDKXRegmonDispatchWrite;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DDKXRegmonDispatchControl;

	// 创建REGMON设备
	status = CreateXRegmonDevice(pDriverObject);
	
	// 设置KEY
	ServiceTable = KeServiceDescriptorTable;
	RealRegSetValueKey = (fnRealRegSetValueKey) SYSCALL( ZwSetValueKey );
	ProcessNameOffset = GetProcessNameOffset();
	LogWrite("ServiceTable:%X; RealRegSetValueKey:%X; RegSetValueKey:%X\r\n", ServiceTable, RealRegSetValueKey, RegSetValueKey);

	return status;
}

// 创建设备
// #pragma INITCODE
NTSTATUS CreateXRegmonDevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS			status;
	PDEVICE_OBJECT		pDevObj;
	PDEVICE_EXTENSION	pDevExt;
	UNICODE_STRING		devName;
	UNICODE_STRING		devLink;

	RtlInitUnicodeString(&devName, XREGMON_DEVICE);
	RtlInitUnicodeString(&devLink, XREGMON_LINKNAME);

	status = IoCreateDevice(pDriverObject
		, sizeof(DEVICE_EXTENSION)
		, &devName
		, FILE_DEVICE_UNKNOWN
		, 0
		, TRUE
		, &pDevObj);

	if( !NT_SUCCESS(status) )
	{
		LogWrite("IoCreate Device '%S' Error: %d\r\n", devName.Buffer, status);
		return status;
	}

	pDevObj->Flags |= DO_BUFFERED_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;

	status = IoCreateSymbolicLink(&devLink, &devName);
	if( !NT_SUCCESS(status) )
	{
		LogWrite("Create Symbolic link '%S' Error: %d\r\n", devLink.Buffer, status);
		IoDeleteDevice(pDevObj);
		return status;
	}

	LogWrite("Create device '%S'['%S'] success.\r\n", devName.Buffer, devLink.Buffer);

	return STATUS_SUCCESS;
}
// 御载函数
// #pragma PAGEDCODE
void DDKXRegmonUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	LogWrite("Enter LogDDKUnload\r\n");
	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		UNICODE_STRING			pLinkName;

		RtlInitUnicodeString(&pLinkName, XREGMON_LINKNAME);
		IoDeleteSymbolicLink(&pLinkName);
		pNexObj = pNexObj->NextDevice;
		IoDeleteDevice(pDevExt->pDevice);

		if(NULL != RealRegSetValueKey && SYSCALL(ZwSetValueKey) != RealRegSetValueKey)
			SETSYSCALL(ZwSetValueKey, RealRegSetValueKey);
	}
	LogWrite("Leave LogDDKUnload\r\n");
}

// 日志函数
// #pragma PAGEDCODE
void LogWrite(const char* pFmt, ...)
{
	OBJECT_ATTRIBUTES			obejectAttributes;
	IO_STATUS_BLOCK				iostatus;
	HANDLE						hfile;
	UNICODE_STRING				logFileUnicodeString;
	NTSTATUS					ntStatus;
	PUCHAR						pstr;

	RtlInitUnicodeString(&logFileUnicodeString, XREGMON_LOGFILE);

	InitializeObjectAttributes(&obejectAttributes
		, &logFileUnicodeString
		, OBJ_CASE_INSENSITIVE
		, NULL
		, NULL);
	pstr = (PUCHAR)ExAllocatePool(PagedPool, 1024);

	// 创建文件
	ntStatus = ZwCreateFile(&hfile
		, FILE_APPEND_DATA
		, &obejectAttributes
		, &iostatus
		, NULL
		, FILE_ATTRIBUTE_NORMAL
		, FILE_SHARE_READ
		, FILE_OPEN
		, FILE_SYNCHRONOUS_IO_NONALERT
		, NULL
		, 0);
	if(NT_SUCCESS(ntStatus))
	{
		va_list		varArr;

		va_start(varArr, pFmt);
		_vsnprintf((char *)pstr, 1024, pFmt, varArr);
		va_end(varArr);
		ZwWriteFile(hfile, NULL, NULL, NULL, &iostatus, (void *)pstr
			, strlen((char *)pstr), NULL, NULL);
		ZwClose(hfile);
	}

	ExFreePool(pstr);
}

// 默认派遣函数
// // #pragma PAGEDCODE
NTSTATUS DDKXRegmonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	NTSTATUS status = STATUS_SUCCESS;
	//建立一个字符串数组与IRP类型对应起来
	static char* irpname[] = 
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP",
	};

	UCHAR type = stack->MajorFunction;

	LogWrite("Enter HelloDDKDispatchRoutin\r\n");

	if (type >= arrayof(irpname))
		LogWrite(" - Unknown IRP, major type %X\r\n", type);
	else
		LogWrite("\t%s\r\n", irpname[type]);


	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	LogWrite("Leave HelloDDKDispatchRoutin\r\n");

	return status;
}

// 读派遣函数
// #pragma PAGEDCODE
NTSTATUS DDKXRegmonDispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PDEVICE_EXTENSION		pDevExt			= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	NTSTATUS				status			= STATUS_SUCCESS;
	PIO_STACK_LOCATION		stack			= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					ulReadLength	= stack->Parameters.Read.Length;


	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulReadLength;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

// 写认派遣函数
// #pragma PAGEDCODE
NTSTATUS DDKXRegmonDispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS				status			= STATUS_SUCCESS;
	PDEVICE_EXTENSION		pDevExt			= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PIO_STACK_LOCATION		stack			= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					ulWriteLength	= stack->Parameters.Write.Length;
	ULONG					ulWriteOffset	= (ULONG)stack->Parameters.Write.ByteOffset.QuadPart;


	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulWriteLength;
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	return STATUS_SUCCESS;
}

// IOCONTROL例程
// #pragma PAGEDCODE
NTSTATUS DDKXRegmonDispatchControl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	
	NTSTATUS				status		= STATUS_SUCCESS;
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					nInLen		= stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG					nOutLen		= stack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG					code		= stack->Parameters.DeviceIoControl.IoControlCode;
	PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	LogWrite("[IOBufferDDKDispatchControl] recv: %s\r\n", pIrp->AssociatedIrp.SystemBuffer);

	switch(code)
	{
	case IOCTRL_REGMON_START:
		SETSYSCALL( ZwSetValueKey, RegSetValueKey );
		break;
	case IOCTRL_REGMON_STOP:
		SETSYSCALL( ZwSetValueKey, RealRegSetValueKey );
		break;
	default:
		LogWrite("[IOBufferDDKDispatchControl] unknown code: %X.\r\n", code);
		break;
	}


	pIrp->IoStatus.Information = nOutLen;
	pIrp->IoStatus.Status = status;
	LogWrite("[IOBufferDDKDispatchControl] write byte: %d.\r\n", nOutLen);

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// REGMON
PCHAR GetProcess( PCHAR Name )
{
	PEPROCESS       curproc;
	char            *nameptr;
	ULONG           i;

	//
	// We only try and get the name if we located the name offset
	//
	if( ProcessNameOffset ) {

		curproc = PsGetCurrentProcess();
		nameptr   = (PCHAR) curproc + ProcessNameOffset;
		strncpy( Name, nameptr, 16 );

	} else {

		strcpy( Name, "???");
	}

	return Name;
}

// #pragma PAGEDCODE
#define MAXPATHLEN 1024
NTSTATUS RegSetValueKey( IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName,
						IN ULONG TitleIndex, IN ULONG Type, 
						IN PVOID Data, IN ULONG DataSize )
{
	CHAR			szMod[512]				= {0};
	WCHAR			szFullPath[MAXPATHLEN]	= {0};		
	PVOID			pKeyObj					= NULL;
	ULONG			ulRet					= 0;
	PUNICODE_STRING	fullUniName				= NULL;

	if(STATUS_SUCCESS == ObReferenceObjectByHandle(KeyHandle, 0, NULL, KernelMode, &pKeyObj, NULL))
	{
		fullUniName = ExAllocatePool( PagedPool, MAXPATHLEN*2+2*sizeof(ULONG));
		fullUniName->MaximumLength = MAXPATHLEN*2;
		ObQueryNameString(pKeyObj, (POBJECT_NAME_INFORMATION)fullUniName, MAXPATHLEN, &ulRet);
		ObDereferenceObject(pKeyObj);
		wcsncpy(szFullPath, fullUniName->Buffer, MAXPATHLEN);
		ExFreePool(fullUniName);
	}
	LogWrite("Reg set value: %s '%S' \r\n", GetProcess(szMod), szFullPath);
	return RealRegSetValueKey(KeyHandle, ValueName, TitleIndex, Type, Data, DataSize);
}
