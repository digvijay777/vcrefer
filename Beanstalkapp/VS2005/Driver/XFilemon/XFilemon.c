/*
 *	ע�����
 */
#include "XFilemon.h"

//////////////////////////////////////////////////////////////////////////
// ��ں���
#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS		status = STATUS_SUCCESS;
	int				i = 0;

	DbgPrint("Enter DriverEntry\r\n");
	// ע�������������ú������
	for(; i < arrayof(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = DDKXFilemonDispatchRoutine;
	}

	pDriverObject->DriverUnload = DDKXFilemonUnload;

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
	}
	DbgPrint("�����豸\"%S\": %d\r\n", usDevice.Buffer, status);
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
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	NTSTATUS status = STATUS_SUCCESS;
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


	//��һ��IRP�ļ򵥲������������ܶ�IRP�����ӵĲ���
	// ���IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	DbgPrint("Leave HelloDDKDispatchRoutin\r\n");

	return status;
}
