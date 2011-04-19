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
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = IOBufferDDKDispatchWrite;

	// 缓冲区读方式
	status = CreateBufferIODevice(pDriverObject);

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

	RtlInitUnicodeString(&devName, L"\\Device\\DDKCallDriverA");
	RtlInitUnicodeString(&devLink, L"\\??\\CallDriverA");

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
	InitializeListHead(&pDevExt->head);
	KeInitializeTimer(&pDevExt->pollingTimer);
	pDevExt->pollingInterval = RtlConvertLongToLargeInteger(-10 * 1000000);
	KeInitializeDpc(&pDevExt->pollingDPC, PollingTimerDpc, (PVOID)pDevObj );
	pDevExt->curIRP = NULL;
	//KeSetTimer(&pDevExt->pollingTimer, pDevExt->pollingInterval, &pDevExt->pollingDPC);

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
#pragma PAGEDCODE
void IOBufferDDKUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	LogWrite("Enter LogDDKUnload\r\n");
	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		UNICODE_STRING			pLinkName;

		RtlInitUnicodeString(&pLinkName, L"\\??\\CallDriverA");
		IoDeleteSymbolicLink(&pLinkName);
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

	RtlInitUnicodeString(&logFileUnicodeString, L"\\??\\C:\\IO.log");

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

	if(IRP_MJ_CLOSE == type)
	{
		while(FALSE == IsListEmpty(&pDevExt->head))
		{
			PLIST_ENTRY		pList		= RemoveHeadList(&pDevExt->head);
			MyPIRPList*		pData		= CONTAINING_RECORD(pList, MyPIRPList, ListEntey);

			pData->pIrp->IoStatus.Information = 0;
			pData->pIrp->IoStatus.Status = STATUS_SUCCESS;
			IoCancelIrp(pData->pIrp);
			ExFreePool(pData);
		}
// 		KeCancelTimer(&pDevExt->pollingTimer);
// 		if(NULL != pDevExt->curIRP)
// 		{
// 			IoCancelIrp(pDevExt->curIRP);
// 			pDevExt->curIRP = NULL;
// 		}
	}
	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	LogWrite("Leave HelloDDKDispatchRoutin\r\n");

	return status;
}

// 读派遣函数
#pragma PAGEDCODE
NTSTATUS IOBufferDDKDispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PDEVICE_EXTENSION		pDevExt			= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	NTSTATUS				status			= STATUS_SUCCESS;
	PIO_STACK_LOCATION		stack			= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					ulReadLength	= stack->Parameters.Read.Length;
	ULONG					ulReadOffset	= (ULONG)stack->Parameters.Read.ByteOffset.QuadPart;
	MyPIRPList*				pList			= NULL;

	IoMarkIrpPending(pIrp);

	pList = (MyPIRPList *)ExAllocatePool(PagedPool, sizeof(MyPIRPList));
	if(NULL == pList)
	{
		if(ulReadLength > 14)
		{
			strcpy((char *)pIrp->AssociatedIrp.SystemBuffer, "read dispatch");
			ulReadLength = 14;
		}
		else
		{
			ulReadLength = 0;
		}
		pIrp->IoStatus.Information = ulReadLength;
		pIrp->IoStatus.Status = STATUS_SUCCESS;

		return STATUS_SUCCESS;
	}

	InsertTailList(&pDevExt->head, &pList->ListEntey);
	pList->pIrp = pIrp;
// 	pDevExt->curIRP = pIrp;
	KeSetTimer(&pDevExt->pollingTimer, pDevExt->pollingInterval, &pDevExt->pollingDPC);

	return STATUS_PENDING; // 不然会蓝屏
}

// 写认派遣函数
#pragma PAGEDCODE
NTSTATUS IOBufferDDKDispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS				status			= STATUS_SUCCESS;
	PDEVICE_EXTENSION		pDevExt			= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PIO_STACK_LOCATION		stack			= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					ulWriteLength	= stack->Parameters.Write.Length;
	ULONG					ulWriteOffset	= (ULONG)stack->Parameters.Write.ByteOffset.QuadPart;

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulWriteLength;

	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	LogWrite("[IOBufferDDKDispatchRead] write: %d\r\n", status);

	return STATUS_SUCCESS;
}

// 定时器
#pragma LOCKEDCODE
void PollingTimerDpc(IN PKDPC pDpc, PVOID pContext, PVOID SysArg1, PVOID SysArg2)
{
	PDEVICE_OBJECT		pDevObj		= (PDEVICE_OBJECT)pContext;
	if(NULL == pDevObj)
		return;
	PDEVICE_EXTENSION	pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	if(NULL != pDevExt)
	{
		while(FALSE == IsListEmpty(&pDevExt->head))
		{
			PLIST_ENTRY		pList		= RemoveHeadList(&pDevExt->head);
			MyPIRPList*		pData		= CONTAINING_RECORD(pList, MyPIRPList, ListEntey);
			IO_STACK_LOCATION* stack	= IoGetCurrentIrpStackLocation(pData->pIrp);
			ULONG			ulReadLength	= stack->Parameters.Read.Length;

			if(ulReadLength > 14)
			{
				strcpy((char *)pData->pIrp->AssociatedIrp.SystemBuffer, "dpc  dispatch");
				ulReadLength = 14;
			}
			else
			{
				ulReadLength = 0;
			}

			pData->pIrp->IoStatus.Information = ulReadLength;
			pData->pIrp->IoStatus.Status = STATUS_SUCCESS;
			IoCompleteRequest(pData->pIrp, IO_NO_INCREMENT);
			ExFreePool(pData);
		}
	}
	return;
}


