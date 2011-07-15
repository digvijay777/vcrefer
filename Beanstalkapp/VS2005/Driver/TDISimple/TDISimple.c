/*
 *	ע�����
 */
#include "TDISimple.h"

LONG			gLock			= 0;

//////////////////////////////////////////////////////////////////////////
// ��ں���
#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject
								, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS		status = STATUS_SUCCESS;
	int				i = 0;

	KdPrint(("Enter DriverEntry\r\n"));
	// ע�������������ú������
	for(; i < arrayof(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = DDKXRegmonDispatchRoutine;
	}

	pDriverObject->DriverUnload = DDKXRegmonUnload;
// 	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DDKXRegmonDispatchControl;

	// ����REGMON�豸
	status = CreateXRegmonDevice(pDriverObject);

	KdPrint(("Enter DriverEntry: %x:%x:%x\r\n"
		, pDriverObject->DeviceObject
		, ((DEVICE_EXTENSION *)pDriverObject->DeviceObject->DeviceExtension)->pDevice
		, ((DEVICE_EXTENSION *)pDriverObject->DeviceObject->DeviceExtension)->pAttatchDev));

	return status;
}

// �����豸
#pragma INITCODE
NTSTATUS CreateXRegmonDevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS				status;
	PDEVICE_OBJECT			pDevObj;
	PDEVICE_EXTENSION		pDevExt;
	UNICODE_STRING			ustrTcp;

	status = IoCreateDevice(pDriverObject
		, sizeof(DEVICE_EXTENSION)
		, NULL
		, FILE_DEVICE_UNKNOWN
		, 0
		, TRUE
		, &pDevObj);
	if( !NT_SUCCESS(status) )
	{
		KdPrint(("IoCreate Device Error: %d\r\n", status));
		return status;
	}
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	// ���豸
	pDevObj->Flags |= DO_DIRECT_IO;
	RtlInitUnicodeString(&ustrTcp, L"\\Device\\Tcp");
	status = IoAttachDevice(pDevObj, &ustrTcp, &pDevExt->pAttatchDev);
	KdPrint(("Enter CreateXRegmonDevice: %x:%x:%x\r\n"
		, pDevObj, ((DEVICE_EXTENSION *)pDevObj)->pDevice, ((DEVICE_EXTENSION *)pDevObj)->pAttatchDev));
	if(!NT_SUCCESS(status) || NULL == pDevExt->pAttatchDev)
	{
		IoDeleteDevice(pDevObj);
		return status;
	}

	pDevObj->DeviceType = pDevExt->pAttatchDev->DeviceType;
	pDevObj->Characteristics = pDevExt->pAttatchDev->Characteristics;
	pDevObj->Flags &= ~DO_DEVICE_INITIALIZING;
	pDevObj->Flags |= (pDevExt->pAttatchDev->Flags 
		& (DO_DIRECT_IO | DO_BUFFERED_IO));

	return STATUS_SUCCESS;
}
// ��ʱ����
void OnTimer(IN PDEVICE_OBJECT DeviceObject, IN PVOID Context)
{
	KdPrint(("Enter OnTimer: %d.\n", gLock));
	if(gLock > 0)
		return;
	IoStopTimer(DeviceObject);
	IoDeleteDevice(DeviceObject);
	KdPrint(("OnTimer: Delete device.\n"));
}
// ���غ���
#pragma PAGEDCODE
void DDKXRegmonUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	KdPrint(("Enter LogDDKUnload\r\n"));
	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		PDEVICE_OBJECT			pAttatch	= pDevExt->pAttatchDev;

		pNexObj = pNexObj->NextDevice;
		if(NULL != pAttatch)
			IoDetachDevice(pDevExt->pAttatchDev);
		// pDevExt->pAttatchDev = NULL;
		if(0 < gLock)
		{
			IoInitializeTimer(pDevExt->pDevice, OnTimer, NULL);
			IoStartTimer(pDevExt->pDevice);
		}
		else
		{
			IoDeleteDevice(pDevExt->pDevice);
			KdPrint(("Delete device.\n"));
		}
	}
	KdPrint(("Leave LogDDKUnload\r\n"));
}

// Ĭ����ǲ����
#pragma PAGEDCODE
NTSTATUS DDKXRegmonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION	stack		= IoGetCurrentIrpStackLocation(pIrp);
	NTSTATUS			status		= STATUS_SUCCESS;
	PDEVICE_OBJECT		pAttatch	= ((DEVICE_EXTENSION *)pDevObj->DeviceExtension)->pAttatchDev;
	UCHAR				type;

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

	InterlockedIncrement(&gLock);

	type = stack->MajorFunction;


	if (type >= arrayof(irpname))
		KdPrint((" - Unknown IRP, major type %X\r\n", type));
	else
		KdPrint(("\t%s\r\n", irpname[type]));

	if(NULL == pAttatch)
	{
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest( pIrp, IO_NO_INCREMENT );
		InterlockedDecrement(&gLock);
		return STATUS_SUCCESS;
	}
	//��һ��IRP�ļ򵥲������������ܶ�IRP�����ӵĲ���
	KdPrint(("Enter DDKXRegmonDispatchRoutine IoCallDriver\r\n"));

	IoCopyCurrentIrpStackLocationToNext(pIrp);
	IoSetCompletionRoutine(pIrp, DispatchRoutineComplate, pDevObj, TRUE, TRUE, TRUE);
	return IoCallDriver( pAttatch, pIrp );
}
// IOCONTROL����
#pragma PAGEDCODE
NTSTATUS DDKXRegmonDispatchControl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS				status		= STATUS_SUCCESS;
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(pIrp);
	ULONG					nInLen		= stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG					nOutLen		= stack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG					code		= stack->Parameters.DeviceIoControl.IoControlCode;
	PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	KdPrint(("[IOBufferDDKDispatchControl] recv: %s\r\n", pIrp->AssociatedIrp.SystemBuffer));

	IoSkipCurrentIrpStackLocation(pIrp);

	return IoCallDriver(((DEVICE_EXTENSION *)pDevObj)->pAttatchDev, pIrp);
}

#pragma PAGEDCODE
NTSTATUS DispatchRoutineComplate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(Irp);
	int						numkey		= 0;

	KdPrint(("Enter DispatchRoutineComplate. \r\n"));

	if(Irp->PendingReturned)
	{
		IoMarkIrpPending(Irp);
	}


	InterlockedDecrement(&gLock);
	return Irp->IoStatus.Status;
}