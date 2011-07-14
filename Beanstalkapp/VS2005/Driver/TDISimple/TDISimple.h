#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <ntddk.h>
#ifdef __cplusplus
};
#endif


#define PAGEDCODE			code_seg("PAGE")
#define LOCKEDCODE			code_seg()
#define INITCODE			code_seg("INIT")

#define PAGEDDATA			data_seg("PAGE")
#define LOCKEDDATA			data_seg()
#define INITDATA			data_seg("INIT")

#define arrayof(p)		( sizeof(p) / sizeof((p)[0]) )

typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT			pDevice;
	PDEVICE_OBJECT			pAttatchDev;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


NTSTATUS CreateXRegmonDevice(PDRIVER_OBJECT pDriverObject);

void DDKXRegmonUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS DDKXRegmonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DDKXRegmonDispatchControl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

