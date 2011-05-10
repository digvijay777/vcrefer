#include "HelloWDM.h"

#pragma INITCODE
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject
								, IN PUNICODE_STRING pRegistryPath)
{
	pDriverObject->DriverExtension->AddDevice = HelloWDMAddDevice;
	pDriverObject->MajorFunction[IRP_MJ_PNP] = HelloWDMPnp;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] =
		pDriverObject->MajorFunction[IRP_MJ_CREATE] =
		pDriverObject->MajorFunction[IRP_MJ_READ] =
		pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloWDMDispatchRoutine;
	pDriverObject->DriverUnload = HelloWDMUnload;

	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
NTSTATUS HelloWDMAddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject)
{
	PAGED_CODE();

	NTSTATUS			status;
	PDEVICE_OBJECT		fdo;
	UNICODE_STRING		ustrName;

	RtlInitUnicodeString(&ustrName, L"\\Device\\MyWDMDevice");
	status = IoCreateDevice(DriverObject
		, sizeof(DEVICE_EXTENSION)
		, &ustrName
		, FILE_DEVICE_UNKNOWN
		, 0
		, FALSE
		, &fdo);
	if(!NT_SUCCESS(status))
		return status;

	PDEVICE_EXTENSION	pdx		= (PDEVICE_EXTENSION)fdo->DeviceExtension;
	pdx->fdo = fdo;
	pdx->NextStackDevice = IoAttachDeviceToDeviceStack(fdo, PhysicalDeviceObject);
	
	UNICODE_STRING		sysLink;
	RtlInitUnicodeString(&sysLink, L"\\DosDevices\\HelloWDM");

	pdx->ustrDeviceName = ustrName;
	pdx->ustrSymLinkName = sysLink;
	status = IoCreateSymbolicLink(&sysLink, &ustrName);
	
	if(!NT_SUCCESS(status))
	{
		IoDeleteSymbolicLink(&pdx->ustrSymLinkName);
		status = IoCreateSymbolicLink(&sysLink, &ustrName);
	}

	fdo->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
	fdo->Flags &= ~DO_DEVICE_INITIALIZING;
	
	return status;
}

#pragma PAGEDCODE
NTSTATUS HelloWDMPnp(IN PDEVICE_OBJECT fdo, IN PIRP Irp)
{
	PAGED_CODE();

	NTSTATUS		status;
	PDEVICE_EXTENSION		pdx		= (PDEVICE_EXTENSION)fdo->DeviceExtension;
	PIO_STACK_LOCATION		stack	= IoGetCurrentIrpStackLocation(Irp);

	if(IRP_MN_REMOVE_DEVICE == stack->MinorFunction)
	{
		return HandleRemoveDevice(pdx, Irp);
	}

	return HandlePnpHandler(pdx, Irp);
}

#pragma PAGEDCODE 
NTSTATUS HandlePnpHandler(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();

	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(pdx->NextStackDevice, Irp);
}

#pragma PAGEDCODE
NTSTATUS HandleRemoveDevice(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	NTSTATUS		status	= HandlePnpHandler(pdx, Irp);
	IoDeleteSymbolicLink(&pdx->ustrSymLinkName);

	if(pdx->NextStackDevice)
		IoDetachDevice(pdx->NextStackDevice);

	IoDeleteDevice(pdx->fdo);
	return status;
}

#pragma PAGEDCODE
NTSTATUS HelloWDMDispatchRoutine(IN PDEVICE_OBJECT fdo, IN PIRP Irp)
{
	PAGED_CODE();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
void HelloWDMUnload(IN PDRIVER_OBJECT DriverObject)
{
	PAGED_CODE();

}

