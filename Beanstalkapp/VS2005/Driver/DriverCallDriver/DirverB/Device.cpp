#include "Device.h"
#include <stdio.h>
#include <stdarg.h>


// 入口函数
#pragma INITCODE
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject
								, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS		status = STATUS_SUCCESS;

	LogWrite("Enter DriverEntry\r\n");
	// 注册其他驱动调用函数入口
	for(int i = 0; i < arraysize(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = IOBufferDDKDispatchRoutine;
	}

	pDriverObject->DriverUnload = IOBufferDDKUnload;
 	pDriverObject->MajorFunction[IRP_MJ_READ] = IOBufferDDKDispatchRead;
// 	pDriverObject->MajorFunction[IRP_MJ_WRITE] = IOBufferDDKDispatchWrite;

	// 得到驱动
	UNICODE_STRING			driveA;
	PDEVICE_OBJECT			DeviceA;
	PFILE_OBJECT			FileObjectA;
	
	RtlInitUnicodeString(&driveA, L"\\Device\\DDKCallDriverA");
	status = IoGetDeviceObjectPointer(&driveA, FILE_ALL_ACCESS, &FileObjectA, &DeviceA);
	if(!NT_SUCCESS(status))
	{
		LogWrite("DriverB: IoGetDeviceObjectPointer() 0x%X\r\n", status);
		return status;
	}
	// 缓冲区读方式
	status = CreateBufferIODevice(pDriverObject);
	// 是否操作成功
	if(!NT_SUCCESS(status))
	{
		ObDereferenceObject(FileObjectA);
		LogWrite("IoCreateDevice() 0x%X!\r\n", status);
		return status;
	}
	// 扩展设备
	PDEVICE_EXTENSION		pdx		= (PDEVICE_EXTENSION)pDriverObject->DeviceObject->DeviceExtension;

	pdx->pTargetDevice = IoAttachDeviceToDeviceStack(pdx->pDevice, DeviceA);
	if(!pdx->pTargetDevice)
	{
		ObDereferenceObject(FileObjectA);
		IoDeleteDevice(pdx->pDevice);
		LogWrite("IoAttachDeviceToDeviceStack() 0x%X!\r\n", status);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	// 设置类型
	pdx->pDevice->DeviceType = pdx->pTargetDevice->DeviceType;
	pdx->pDevice->Characteristics = pdx->pTargetDevice->Characteristics;
	pdx->pDevice->Flags &= ~DO_DEVICE_INITIALIZING;
	pdx->pDevice->Flags |= (pdx->pTargetDevice->Flags & (DO_DIRECT_IO | DO_BUFFERED_IO));

 	ObDereferenceObject(FileObjectA);

	LogWrite("DriverEnter end\r\n");
	return status;
}
// 创建设备
#pragma INITCODE
NTSTATUS CreateBufferIODevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS			status;
	PDEVICE_OBJECT		pDevObj;
	PDEVICE_EXTENSION	pDevExt;
	UNICODE_STRING		devName;
	UNICODE_STRING		devLink;

	RtlInitUnicodeString(&devName, L"\\Device\\DDKCallDriverB");
	RtlInitUnicodeString(&devLink, L"\\??\\CallDriverB");

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

	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->pTargetDevice = NULL;

	return STATUS_SUCCESS;
}
// 御载函数
#pragma PAGEDCODE
void IOBufferDDKUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	LogWrite("Enter LogDDKUnload\r\n");
	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;

		if(NULL != pDevExt->pTargetDevice)
			IoDetachDevice(pDevExt->pTargetDevice);
		pNexObj = pNexObj->NextDevice;
		IoDeleteDevice(pDevExt->pDevice);
	}
	LogWrite("Leave LogDDKUnload\r\n");
}

// 日志函数
#pragma PAGEDCODE
void LogWrite(const char* pFmt, ...)
{
	OBJECT_ATTRIBUTES			obejectAttributes;
	IO_STATUS_BLOCK				iostatus;
	HANDLE						hfile;
	UNICODE_STRING				logFileUnicodeString;
	NTSTATUS					ntStatus;
	PUCHAR						pstr;

	RtlInitUnicodeString(&logFileUnicodeString, L"\\??\\C:\\IOB.log");

	InitializeObjectAttributes(&obejectAttributes
		, &logFileUnicodeString
		, OBJ_CASE_INSENSITIVE
		, NULL
		, NULL);
	pstr = (PUCHAR)ExAllocatePool(PagedPool, 1024);

	// 创建文件
	ntStatus = ZwCreateFile(&hfile
		, /*GENERIC_WRITE |*/ FILE_APPEND_DATA
		, &obejectAttributes
		, &iostatus
		, NULL
		, FILE_ATTRIBUTE_NORMAL
		, FILE_SHARE_READ
		, FILE_OPEN_IF
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
#pragma PAGEDCODE
NTSTATUS IOBufferDDKDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	LogWrite("Enter HelloDDKDispatchRoutin\r\n");

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	PDEVICE_EXTENSION	pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
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
	if (type >= arraysize(irpname))
		LogWrite(" - Unknown IRP, major type %X\r\n", type);
	else
		LogWrite("\t%s\r\n", irpname[type]);

	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	IoSkipCurrentIrpStackLocation(pIrp);

	status = IoCallDriver(pDevExt->pTargetDevice, pIrp);

// 	pIrp->IoStatus.Status = status;
// 	pIrp->IoStatus.Information = 0;	// bytes xfered
// 	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	//LogWrite("Leave HelloDDKDispatchRoutin\r\n");

	return status;
}

#pragma PAGEDCODE
NTSTATUS MyIoCmpletion(PDEVICE_OBJECT pDevObj, PIRP pIrp, PVOID Context)
{
// 	if(pIrp->PendingReturned)
// 	{
// 		IoMarkIrpPending(pIrp);
// 	}
// 
// 	IO_STACK_LOCATION*		stack		= IoGetCurrentIrpStackLocation(pIrp);
// 	ULONG					ulRead		= stack->Parameters.Read.Length;
// 	ULONG					ulWrite		= pIrp->IoStatus.Information;
// 
// 	if(ulWrite+5 < ulRead)
// 	{
// 		strcat((char *)pIrp->AssociatedIrp.SystemBuffer, "-[B]");
// 		pIrp->IoStatus.Information = ulWrite + 5;
// 	}
// 
// 	pIrp->IoStatus.Status = STATUS_SUCCESS;
// 	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	if(pIrp->PendingReturned)
	{
		KeSetEvent((PKEVENT)Context, IO_NO_INCREMENT, FALSE);
	}

	return STATUS_MORE_PROCESSING_REQUIRED;
}

// 读派遣函数
#pragma PAGEDCODE
NTSTATUS IOBufferDDKDispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PDEVICE_EXTENSION	pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	NTSTATUS status = STATUS_SUCCESS;
	KEVENT				kEvent;

	KeInitializeEvent(&kEvent, NotificationEvent, FALSE);
	// 完成IRP
	//IoSkipCurrentIrpStackLocation(pIrp);
	IoCopyCurrentIrpStackLocationToNext(pIrp);
	IoSetCompletionRoutine(pIrp, MyIoCmpletion, &kEvent, TRUE, TRUE, TRUE);
	status = IoCallDriver(pDevExt->pTargetDevice, pIrp);
	if(STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
		status = pIrp->IoStatus.Status;
	}

	IO_STACK_LOCATION*		stack		= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					ulRead		= stack->Parameters.Read.Length;
	ULONG					ulWrite		= pIrp->IoStatus.Information;

	LogWrite("DriveA Read: (%d/%d)%s\r\n", ulWrite, ulRead, pIrp->AssociatedIrp.SystemBuffer);
	if( ulWrite > 0 && (ulWrite+5) < ulRead )
	{
		strcat((char *)pIrp->AssociatedIrp.SystemBuffer, "-[B]");
		pIrp->IoStatus.Information = ulWrite + 5;
	}
 
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status; // 不然会蓝屏
}
/*
// 写认派遣函数
#pragma PAGEDCODE
NTSTATUS IOBufferDDKDispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PDEVICE_EXTENSION	pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	IoSkipCurrentIrpStackLocation(pIrp);

	status = IoCallDriver(pDevExt->pTargetDevice, pIrp);

	return status; // 不然会蓝屏
}*/


