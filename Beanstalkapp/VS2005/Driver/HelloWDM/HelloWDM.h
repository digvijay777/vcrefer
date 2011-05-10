#pragma once

extern "C"
{
#include <wdm.h>
};

typedef struct _DEVICE_EXTENSION
{
	PDEVICE_OBJECT		fdo;
	PDEVICE_OBJECT		NextStackDevice;
	UNICODE_STRING		ustrDeviceName;
	UNICODE_STRING		ustrSymLinkName;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define PAGEDCODE		code_seg("PAGE")
#define LOCKEDCODE		code_seg()
#define INITCODE		code_seg("INIT")

#define PAGEDDATA		data_seg("PAGE")
#define LOCKEDDATA		data_seg()
#define INITDATA		data_seg("INIT")


#define arrayof(x)		( sizeof((x)) / sizeof((x)[0]) )

NTSTATUS HelloWDMAddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject);
NTSTATUS HelloWDMPnp(IN PDEVICE_OBJECT fdo, IN PIRP Irp);
NTSTATUS HelloWDMDispatchRoutine(IN PDEVICE_OBJECT fdo, IN PIRP Irp);
void HelloWDMUnload(IN PDRIVER_OBJECT DriverObject);

NTSTATUS HandleRemoveDevice(PDEVICE_EXTENSION pdx, PIRP Irp);
NTSTATUS HandlePnpHandler(PDEVICE_EXTENSION pdx, PIRP Irp);


