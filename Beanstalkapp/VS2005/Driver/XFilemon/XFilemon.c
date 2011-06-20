/*
 *	注册表监控
 */
#include "XFilemon.h"
#include "FastIoFunction.h"

//////////////////////////////////////////////////////////////////////////
// 入口函数
#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS					status				= STATUS_SUCCESS;
	int							i					= 0;

	DbgPrint("Enter DriverEntry\r\n");
	// 注册其他驱动调用函数入口
	for(; i < arrayof(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = DDKXFilemonDispatchRoutine;
	}
	pDriverObject->DriverUnload = DDKXFilemonUnload;
	// 文件过滤
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = SfCreate;
	pDriverObject->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = SfCreate;
	pDriverObject->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = SfCreate;
	pDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = SfFsControl;
	pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = SfCleanupClose;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SfCleanupClose;
	// 创建REGMON设备
	status = CreateXFilemonDevice(pDriverObject);
	
	return status;
}

// 创建设备
#pragma INITCODE
NTSTATUS CreateXFilemonDevice(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING				usDevice;
	NTSTATUS					status;
	DEVICE_OBJECT*				devObj;
	PFAST_IO_DISPATCH			fastIoDispatch		= NULL;

	RtlInitUnicodeString( &usDevice, L"\\FileSystem\\Filters\\XFilter" );
	status = IoCreateDevice(pDriverObject
		, sizeof(DEVICE_EXTENSION)
		, &usDevice
		, FILE_DEVICE_DISK_FILE_SYSTEM
		, FILE_DEVICE_SECURE_OPEN
		, FALSE
		, &devObj);
	// 如果因为路径没找到生成失败
	if(STATUS_OBJECT_PATH_NOT_FOUND == status)
	{
		// 这是因为一些低版本的操作系统没有\FileSystem\Filters\这个目录
		// 如果没有，我们则改变位置，生成到\FileSystem\下.
		RtlInitUnicodeString( &usDevice, L"\\FileSystem\\XFilterCDO" );
		status = IoCreateDevice(pDriverObject
			, sizeof(DEVICE_EXTENSION)
			, &usDevice
			, FILE_DEVICE_DISK_FILE_SYSTEM
			, FILE_DEVICE_SECURE_OPEN
			, FALSE
			, &devObj);
	}
	if(NT_SUCCESS(status))
	{
		PDEVICE_EXTENSION		pExt		= (PDEVICE_EXTENSION)devObj->DeviceExtension;

		pExt->pDevice = devObj;
		pExt->AttachedToDeviceObject = NULL;
	}
	DbgPrint("Create device\"%S\": %d\r\n", usDevice.Buffer, status);
	// fast io function
	fastIoDispatch = ExAllocatePoolWithTag(NonPagedPool, sizeof(FAST_IO_DISPATCH), SFLT_POOL_TAG);
	if(NULL == fastIoDispatch)
	{
		IoDeleteDevice(devObj);
		DbgPrint("ExAllocatePoolWithTag SFLT_POOL_TAG failed.");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlZeroMemory(fastIoDispatch, sizeof(FAST_IO_DISPATCH));
	fastIoDispatch->SizeOfFastIoDispatch = sizeof(FAST_IO_DISPATCH);

	fastIoDispatch->FastIoCheckIfPossible = SfFastIoCheckIfPossible;
	fastIoDispatch->AcquireFileForNtCreateSection = SfAcquireFileForNtCreateSection;
	fastIoDispatch->AcquireForCcFlush = SfAcquireForCcFlush;
	fastIoDispatch->FastIoDetachDevice = SfFastIoDetachDevice;
	fastIoDispatch->FastIoDeviceControl = SfFastIoDeviceControl;
	fastIoDispatch->FastIoLock = SfFastIoLock;
	fastIoDispatch->FastIoQueryBasicInfo = SfFastIoQueryBasicInfo;
	fastIoDispatch->FastIoQueryNetworkOpenInfo = SfFastIoQueryNetworkOpenInfo;
	fastIoDispatch->FastIoQueryOpen = SfFastIoQueryOpen;
	fastIoDispatch->FastIoQueryStandardInfo = SfFastIoQueryStandardInfo;
	fastIoDispatch->FastIoRead = SfFastIoRead;
	fastIoDispatch->FastIoReadCompressed = SfFastIoReadCompressed;
	fastIoDispatch->FastIoUnlockAll = SfFastIoUnlockAll;
	fastIoDispatch->FastIoUnlockAllByKey = SfFastIoUnlockAllByKey;
	fastIoDispatch->FastIoUnlockSingle = SfFastIoUnlockSingle;
	fastIoDispatch->FastIoWrite = SfFastIoWrite;
	fastIoDispatch->FastIoWriteCompressed = SfFastIoWriteCompressed;
	fastIoDispatch->MdlRead = SfMdlRead;
	fastIoDispatch->MdlReadComplete = SfMdlReadComplete;
	fastIoDispatch->MdlReadCompleteCompressed = SfMdlReadCompleteCompressed;
	fastIoDispatch->MdlWriteComplete = SfMdlWriteComplete;
	fastIoDispatch->MdlWriteCompleteCompressed = SfMdlWriteCompleteCompressed;
	fastIoDispatch->PrepareMdlWrite = SfPrepareMdlWrite;
	fastIoDispatch->ReleaseFileForNtCreateSection = SfReleaseFileForNtCreateSection;
	fastIoDispatch->ReleaseForCcFlush = SfReleaseForCcFlush;
	fastIoDispatch->ReleaseForModWrite = SfReleaseForModWrite;

	pDriverObject->FastIoDispatch = fastIoDispatch;

	status = SfAttatchDevice((PDEVICE_EXTENSION)devObj->DeviceExtension);

	if(!NT_SUCCESS(status))
	{
		pDriverObject->FastIoDispatch = NULL;
		ExFreePoolWithTag(fastIoDispatch, SFLT_POOL_TAG);
		IoDeleteDevice(devObj);
		DbgPrint("CreateXFilemonDevice: attachdevice failed: %d\n", status);
		return status;
	}

	devObj->DeviceType = ((PDEVICE_EXTENSION)devObj->DeviceExtension)->AttachedToDeviceObject->DeviceType;
	devObj->Characteristics = ((PDEVICE_EXTENSION)devObj->DeviceExtension)->AttachedToDeviceObject->Characteristics;
	devObj->Flags &= ~DO_DEVICE_INITIALIZING;
	devObj->Flags |= (((PDEVICE_EXTENSION)devObj->DeviceExtension)->AttachedToDeviceObject->Flags 
		& (DO_DIRECT_IO | DO_BUFFERED_IO));

	return status;
}
// 御载函数
#pragma PAGEDCODE
void DDKXFilemonUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	DbgPrint("Enter LogDDKUnload\r\n");
	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		
		pNexObj = pNexObj->NextDevice;
		if(NULL != pDevExt->AttachedToDeviceObject)
			IoDetachDevice(pDevExt->AttachedToDeviceObject);
		IoDeleteDevice(pDevExt->pDevice);
	}
	DbgPrint("Leave LogDDKUnload\r\n");
}

// 默认派遣函数
// // #pragma PAGEDCODE
NTSTATUS DDKXFilemonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(pIrp);
	PDEVICE_EXTENSION		pExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

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

	DbgPrint("Enter HelloDDKDispatchRoutin\r\n");

	if (type >= arrayof(irpname))
		DbgPrint(" - Unknown IRP, major type %X\r\n", type);
	else
		DbgPrint("\t%s\r\n", irpname[type]);

	/* 过滤驱动 */
	IoSkipCurrentIrpStackLocation( pIrp );

	return IoCallDriver(pExt->AttachedToDeviceObject, pIrp);
}

/************************************************************************/
/* 文件过滤处理函数                                                      */
/************************************************************************/
NTSTATUS SfCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	return STATUS_SUCCESS;
}
NTSTATUS SfFsControl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	return STATUS_SUCCESS;
}
NTSTATUS SfCleanupClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	return STATUS_SUCCESS;
}

NTSTATUS SfAttatchDevice(PDEVICE_EXTENSION pex)
{
	UNICODE_STRING			uclink;
	OBJECT_ATTRIBUTES		objectattribute;
	HANDLE					hSymbolc;
	NTSTATUS				ntStatus;
	ULONG					uLen;
	UNICODE_STRING			ucDevice;
	WCHAR					szBuff[128]				= {0};
	PFILE_OBJECT			fileobject				= NULL;
	PDEVICE_OBJECT			deviceobject			= NULL;

	DbgPrint("Open 'c:' Link\n");
	RtlInitUnicodeString(&uclink, L"\\??\\c:");
	InitializeObjectAttributes(&objectattribute
		, &uclink
		, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE
		, NULL, NULL);
	ntStatus = ZwOpenSymbolicLinkObject(&hSymbolc, FILE_ALL_ACCESS, &objectattribute);
	if(!NT_SUCCESS(ntStatus))
		return ntStatus;

	DbgPrint("Get 'c:' device.\n");
	ucDevice.Buffer = szBuff;
	ucDevice.Length = arrayof(szBuff);
	ntStatus = ZwQuerySymbolicLinkObject(hSymbolc, &ucDevice, &uLen);
	DbgPrint("Get 'c:' device name: %S\n", szBuff);
	if(!NT_SUCCESS(ntStatus))
		return ntStatus;

	DbgPrint("Open 'c:' device\n");
	ntStatus = IoGetDeviceObjectPointer(&ucDevice, SYNCHRONIZE|FILE_ANY_ACCESS, &fileobject, &deviceobject);
	if(!NT_SUCCESS(ntStatus))
		return ntStatus;

	DbgPrint("attach device to device stack.\n");
	pex->AttachedToDeviceObject = IoAttachDeviceToDeviceStack(pex->pDevice, deviceobject);
	ObDereferenceObject(fileobject);
	if(pex->AttachedToDeviceObject)
	{
		DbgPrint("attach 'c:' success.\n");
		pex->AttachedToDeviceObject = deviceobject;
	}
	else
	{
		DbgPrint("attach 'c:' faild.\n");
		return -1;
	}

	DbgPrint("Enter SfAttatchDevice\n");
	return STATUS_SUCCESS;
}