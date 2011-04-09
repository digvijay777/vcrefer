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
	pDriverObject->MajorFunction[IRP_MJ_QUERY_INFORMATION] = IOBufferDDKQueryInformation;

	// 缓冲区读方式
	status = CreateBufferIODevice(pDriverObject);
	// 直接记方式
	status = CreateDirectIODevice(pDriverObject);
	// IO读取练习
	status = CreateControlIODevice(pDriverObject);

	LogWrite("DriverEnter end\r\n");
	return status;
}
// 创建设备
#pragma PAGEDCODE
NTSTATUS CreateBufferIODevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS			status;
	PDEVICE_OBJECT		pDevObj;
	PDEVICE_EXTENSION	pDevExt;
	UNICODE_STRING		devName;
	UNICODE_STRING		devLink;

	RtlInitUnicodeString(&devName, L"\\Device\\DDKTestIOBuffer");
	RtlInitUnicodeString(&devLink, L"\\??\\TestIOBuffer");

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
	pDevExt->ustrDeviceName = devName;
	pDevExt->ustrSymLinkName = devLink;

	pDevExt->nBufferMaxSize = 2048;
	pDevExt->nOffset = 0;
	pDevExt->pBuffer = (PUCHAR)ExAllocatePool(PagedPool, pDevExt->nBufferMaxSize);

	if(NULL ==pDevExt->pBuffer)
	{
		LogWrite("ExAllocate %d Error.\r\n", pDevExt->nBufferMaxSize);
		IoDeleteDevice(pDevObj);
		return STATUS_DEVICE_NOT_CONNECTED;
	}

	status = IoCreateSymbolicLink(&devLink, &devName);
	if( !NT_SUCCESS(status) )
	{
		LogWrite("Create Symbolic link '%S' Error: %d\r\n", devLink.Buffer, status);
		ExFreePool(pDevExt->pBuffer);
		IoDeleteDevice(pDevObj);
		return status;
	}

	LogWrite("Create device '%S'['%S'] success.\r\n", devName.Buffer, devLink.Buffer);

	return STATUS_SUCCESS;
}
// 创建设备
#pragma PAGEDCODE
NTSTATUS CreateDirectIODevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS			status;
	PDEVICE_OBJECT		pDevObj;
	PDEVICE_EXTENSION	pDevExt;
	UNICODE_STRING		devName;
	UNICODE_STRING		devLink;

	RtlInitUnicodeString(&devName, L"\\Device\\DDKTestIODrect");
	RtlInitUnicodeString(&devLink, L"\\??\\TestIODrect");

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

	pDevObj->Flags |= DO_DIRECT_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	pDevExt->ustrSymLinkName = devLink;

	pDevExt->nBufferMaxSize = 2048;
	pDevExt->nOffset = 0;
	pDevExt->pBuffer = (PUCHAR)ExAllocatePool(PagedPool, pDevExt->nBufferMaxSize);

	if(NULL ==pDevExt->pBuffer)
	{
		LogWrite("ExAllocate %d Error.\r\n", pDevExt->nBufferMaxSize);
		IoDeleteDevice(pDevObj);
		return STATUS_DEVICE_NOT_CONNECTED;
	}

	status = IoCreateSymbolicLink(&devLink, &devName);
	if( !NT_SUCCESS(status) )
	{
		LogWrite("Create Symbolic link '%S' Error: %d\r\n", devLink.Buffer, status);
		ExFreePool(pDevExt->pBuffer);
		IoDeleteDevice(pDevObj);
		return status;
	}

	LogWrite("Create device '%S'['%S'] success.\r\n", devName.Buffer, devLink.Buffer);

	return STATUS_SUCCESS;
}

// 创建设备
NTSTATUS CreateControlIODevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS			status;
	PDEVICE_OBJECT		pDevObj;
	PDEVICE_EXTENSION	pDevExt;
	UNICODE_STRING		devName;
	UNICODE_STRING		devLink;

	RtlInitUnicodeString(&devName, L"\\Device\\DDKTestIOControl");
	RtlInitUnicodeString(&devLink, L"\\??\\TestIOControl");

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

	pDevObj->Flags |= DO_DIRECT_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	pDevExt->ustrSymLinkName = devLink;

	pDevExt->nBufferMaxSize = 2048;
	pDevExt->nOffset = 0;
	pDevExt->pBuffer = (PUCHAR)ExAllocatePool(PagedPool, pDevExt->nBufferMaxSize);

	if(NULL ==pDevExt->pBuffer)
	{
		LogWrite("ExAllocate %d Error.\r\n", pDevExt->nBufferMaxSize);
		IoDeleteDevice(pDevObj);
		return STATUS_DEVICE_NOT_CONNECTED;
	}

	status = IoCreateSymbolicLink(&devLink, &devName);
	if( !NT_SUCCESS(status) )
	{
		LogWrite("Create Symbolic link '%S' Error: %d\r\n", devLink.Buffer, status);
		ExFreePool(pDevExt->pBuffer);
		IoDeleteDevice(pDevObj);
		return status;
	}

	LogWrite("Create device '%S'['%S'] success.\r\n", devName.Buffer, devLink.Buffer);

	// 设置例程
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IOBufferDDKDispatchControl;

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
		UNICODE_STRING			pLinkName	= pDevExt->ustrSymLinkName;

		if(NULL != pDevExt->pBuffer)
		{
			ExFreePool(pDevExt->pBuffer);
			pDevExt->pBuffer = NULL;
		}
		//LogWrite("[LogDDKUnload] '%S',", pLinkName.Buffer);
		//IoDeleteSymbolicLink(&pLinkName);
		//RtlInitUnicodeString(&pLinkName, L"\\??\\TestIOBuffer");
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
	UNICODE_STRING			ustrBufferDev;
	UNICODE_STRING			ustrDrectDev;
	UNICODE_STRING			ustrIOCtrlDev;

	RtlInitUnicodeString(&ustrBufferDev, L"\\Device\\DDKTestIOBuffer");
	RtlInitUnicodeString(&ustrDrectDev, L"\\Device\\DDKTestIODrect");
	RtlInitUnicodeString(&ustrIOCtrlDev, L"\\Device\\DDKTestIOControl");

	if(ulReadOffset >= pDevExt->nOffset)
	{
		status = STATUS_END_OF_FILE;
		ulReadLength = 0;
	}
	else 
	{
		if(ulReadLength+ulReadOffset > pDevExt->nOffset)
			ulReadLength = pDevExt->nOffset - ulReadOffset;
		if(RtlCompareUnicodeString(&pDevExt->ustrDeviceName, &ustrBufferDev, TRUE) == 0)
		{
			memcpy(pIrp->AssociatedIrp.SystemBuffer
				, pDevExt->pBuffer + ulReadOffset
				, ulReadLength);
		}
		else if( RtlCompareUnicodeString(&pDevExt->ustrDeviceName, &ustrDrectDev, TRUE) == 0
			|| RtlCompareUnicodeString(&pDevExt->ustrDeviceName, &ustrIOCtrlDev, TRUE) == 0 )
		{
			// 得到锁定缓冲区的首地址长度
			ULONG	mdl_length	= MmGetMdlByteCount(pIrp->MdlAddress);
			// 得到缓冲区的首地址
			PVOID	mdl_address = MmGetMdlVirtualAddress(pIrp->MdlAddress);
			// 得到缓冲区的偏移量
			ULONG	mdl_offset = MmGetMdlByteOffset(pIrp->MdlAddress);

			LogWrite("mdl_address:0X%08X\r\n", mdl_address);
			LogWrite("mdl_length: %d\r\n", mdl_length);
			LogWrite("mdl_offset:%d\r\n", mdl_offset);
			if(mdl_length != stack->Parameters.Read.Length)
			{
				// mdl 的长度与读取长度不一致时
				ulReadLength = 0;
				status = STATUS_UNSUCCESSFUL;
			}
			else
			{
				// 是到MDL在内核模式下的映射
				PVOID	kernal_address = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress
					, NormalPagePriority);
				LogWrite("mdl_address: 0X%08X\r\n", kernal_address);
				memcpy(kernal_address
					, pDevExt->pBuffer + ulReadOffset
					, ulReadLength);
			}

		}
		else
		{
			LogWrite("[IOBufferDDKDispatchRead] bad device name '%S'.\r\n", pDevExt->ustrDeviceName.Buffer);
			status = STATUS_BAD_DEVICE_TYPE;
			ulReadLength = 0;
		}
	}

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulReadLength;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	LogWrite("[IOBufferDDKDispatchRead] device '%S' read: %d\r\n", pDevExt->ustrDeviceName.Buffer, ulReadLength);

	return STATUS_SUCCESS;
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
	UNICODE_STRING			ustrBufferDev;
	UNICODE_STRING			ustrDrectDev;

	RtlInitUnicodeString(&ustrBufferDev, L"\\Device\\DDKTestIOBuffer");
	RtlInitUnicodeString(&ustrDrectDev, L"\\Device\\DDKTestIODrect");

	if(ulWriteOffset+ulWriteLength >= pDevExt->nBufferMaxSize)
	{
		status = STATUS_FILE_INVALID;
		ulWriteLength = 0;
	}
	else
	{
		ulWriteLength = min(pDevExt->nBufferMaxSize - ulWriteOffset, ulWriteLength);
		pDevExt->nOffset = ulWriteOffset + ulWriteLength;
		if(RtlCompareUnicodeString(&ustrBufferDev, &pDevExt->ustrDeviceName, TRUE) == 0)
		{
			memcpy(pDevExt->pBuffer + ulWriteOffset
				, pIrp->AssociatedIrp.SystemBuffer
				, ulWriteLength);
			//pIrp->AssociatedIrp.IrpCount = ulWriteLength;
		}
		else if(RtlCompareUnicodeString(&ustrDrectDev, &pDevExt->ustrDeviceName, TRUE) == 0)
		{
			ULONG	mdl_length	= MmGetMdlByteCount(pIrp->MdlAddress);
			if(mdl_length != stack->Parameters.Read.Length)
			{
				// mdl 的长度与读取长度不一致时
				ulWriteLength = 0;
				status = STATUS_UNSUCCESSFUL;
			}
			else
			{
				// 是到MDL在内核模式下的映射
				PVOID	kernal_address = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress
					, NormalPagePriority);
				LogWrite("mdl_address: 0X%08X\r\n", kernal_address);
				memcpy(pDevExt->pBuffer + ulWriteOffset
					, kernal_address
					, ulWriteLength);
			}
		}
		else
		{
			LogWrite("[IOBufferDDKDispatchWrite] bad device name '%S'.\r\n", pDevExt->ustrDeviceName.Buffer);
			status = STATUS_BAD_DEVICE_TYPE;
			ulWriteLength = 0;
		}
	}

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulWriteLength;
	//pIrp->IoStatus.Pointer = (PVOID)pDevExt->nOffset;
// 	PFILE_STANDARD_INFORMATION		stdInfo		= (PFILE_STANDARD_INFORMATION)pIrp->AssociatedIrp.SystemBuffer;
// 	stdInfo->EndOfFile = RtlConvertLongToLargeInteger(pDevExt->nOffset);
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	LogWrite("[IOBufferDDKDispatchRead] device '%S' write: %d\r\n", pDevExt->ustrDeviceName.Buffer, status);

	return STATUS_SUCCESS;
}

NTSTATUS IOBufferDDKQueryInformation(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(pIrp);
	FILE_INFORMATION_CLASS	info		= stack->Parameters.QueryFile.FileInformationClass;

	if(FileStandardInformation == info)
	{
		LogWrite("[IOBufferDDKQueryInformation] FileStandardInformation device '%S'\r\n", pDevExt->ustrDeviceName.Buffer);
		PFILE_STANDARD_INFORMATION		stdInfo		= (PFILE_STANDARD_INFORMATION)pIrp->AssociatedIrp.SystemBuffer;

		stdInfo->Directory = FALSE;
		stdInfo->EndOfFile = RtlConvertLongToLargeInteger(pDevExt->nOffset);
	}

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = stack->Parameters.QueryFile.Length;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	LogWrite("[IOBufferDDKQueryInformation] device '%S'\r\n", pDevExt->ustrDeviceName.Buffer);
	return STATUS_SUCCESS;
}

// IOCONTROL例程
NTSTATUS IOBufferDDKDispatchControl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	LogWrite("[IOBufferDDKDispatchControl] login.\r\n");
	
	NTSTATUS				status		= STATUS_SUCCESS;
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					nInLen		= stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG					nOutLen		= stack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG					code		= stack->Parameters.DeviceIoControl.IoControlCode;
	PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	LogWrite("[IOBufferDDKDispatchControl] recv: %s\r\n", pIrp->AssociatedIrp.SystemBuffer);
	switch(code)
	{
	case IOCTRL_TEST1:	// 缓冲区方式
		{
			UCHAR*		InputBuffer		= (UCHAR *)pIrp->AssociatedIrp.SystemBuffer;
			
			if(nInLen > pDevExt->nBufferMaxSize)
				nInLen = pDevExt->nBufferMaxSize;
			memcpy(pDevExt->pBuffer, InputBuffer, nInLen);
			pDevExt->nOffset = nInLen;
			// 输出写入字节数
			UCHAR*		OutBuffer		= (UCHAR *)pIrp->AssociatedIrp.SystemBuffer;
			if(nOutLen >= 4)
			{
				nOutLen = 4;
				*((LONG*)OutBuffer) = nInLen;
			}
			else
			{
				nOutLen = 0;
			}
		}
		break;
	case IOCTRL_TEST2:	// 直接方式
		{
			UCHAR*		InputBuffer		= (UCHAR *)pIrp->AssociatedIrp.SystemBuffer;

			if(nInLen > pDevExt->nBufferMaxSize)
				nInLen = pDevExt->nBufferMaxSize;
			memcpy(pDevExt->pBuffer, InputBuffer, nInLen);
			pDevExt->nOffset = nInLen;
			// 返回
			UCHAR*		OutBuffer		= (UCHAR *)MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
			if(nOutLen < 4)
			{
				nOutLen = 0;
			}
			else
			{
				*((LONG *)OutBuffer) = nInLen;
				nOutLen = 4;
			}
		}
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