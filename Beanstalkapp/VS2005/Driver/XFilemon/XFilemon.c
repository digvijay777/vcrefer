/*
 *	ע�����
 */
#include "XFilemon.h"
#include "FastIoFunction.h"

//////////////////////////////////////////////////////////////////////////
// ��ں���
#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS					status				= STATUS_SUCCESS;
	int							i					= 0;

	DbgPrint("Enter DriverEntry\r\n");
	// ע�������������ú������
	for(; i < arrayof(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = DDKXFilemonDispatchRoutine;
	}
	pDriverObject->DriverUnload = DDKXFilemonUnload;
	// �ļ�����
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = SfCreate;
	pDriverObject->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = SfCreate;
	pDriverObject->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = SfCreate;
	pDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = SfFsControl;
	pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = SfCleanupClose;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SfCleanupClose;
	// ����REGMON�豸
	status = CreateXFilemonDevice(pDriverObject);
	
	return status;
}

// �����豸
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
	// �����Ϊ·��û�ҵ�����ʧ��
	if(STATUS_OBJECT_PATH_NOT_FOUND == status)
	{
		// ������ΪһЩ�Ͱ汾�Ĳ���ϵͳû��\FileSystem\Filters\���Ŀ¼
		// ���û�У�������ı�λ�ã����ɵ�\FileSystem\��.
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

	return status;
}
// ���غ���
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
		IoDeleteDevice(pDevExt->pDevice);
	}
	DbgPrint("Leave LogDDKUnload\r\n");
}

// Ĭ����ǲ����
// // #pragma PAGEDCODE
NTSTATUS DDKXFilemonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(pIrp);
	PDEVICE_EXTENSION		pExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	//����һ���ַ���������IRP���Ͷ�Ӧ����
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

	/* �������� */
	IoSkipCurrentIrpStackLocation( pIrp );

	return IoCallDriver(pExt->AttachedToDeviceObject, pIrp);
}

/************************************************************************/
/* �ļ����˴�����                                                      */
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


NTSTATUS SfAttachDeviceToDeviceStack (IN PDEVICE_OBJECT SourceDevice, IN PDEVICE_OBJECT TargetDevice)
{
	return IoAttachDeviceToDeviceStack(SourceDevice, TargetDevice);
}