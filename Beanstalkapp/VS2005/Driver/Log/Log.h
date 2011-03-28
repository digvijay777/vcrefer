#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <ntddk.h>
#ifdef __cplusplus
}
#endif

#define PAGEDCODE	code_seg("PAGE")
#define LOCKEDCODE	code_seg()
#define INITCODE	code_seg("INIT")

#define PAGEDDATA	data_seg("PAGE")
#define LOCKEDDATA	data_seg()
#define INITDATA	data_seg("INIT")

#define arraysize(p)	( sizeof(p)/sizeof((p)[0]) )

typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT		pDevice;
	UNICODE_STRING		ustrDeviceName;		// 设备名
	UNICODE_STRING		ustrSymLinkName;	// 符号链接名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


NTSTATUS	CreateDevice(PDRIVER_OBJECT pDriverObject);
void LogDDKUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS	LogDDKDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp);
