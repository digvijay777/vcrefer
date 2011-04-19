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

#define arraysize(p)		( sizeof(p) / sizeof((p)[0]) )

typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT			pDevice;
	PDEVICE_OBJECT			pTargetDevice;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



NTSTATUS CreateBufferIODevice(PDRIVER_OBJECT pDriverObject);

void IOBufferDDKUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS IOBufferDDKDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS IOBufferDDKDispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS IOBufferDDKDispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp);


void PollingTimerDpc(IN PKDPC pDpc, PVOID pContext, PVOID SysArg1, PVOID SysArg2);

void LogWrite(const char* pFmt, ...);

#define IOCTRL_TEST1			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x800 \
	, METHOD_BUFFERED \
	, FILE_ANY_ACCESS)

#define IOCTRL_TEST2			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x801 \
	, METHOD_IN_DIRECT \
	, FILE_ANY_ACCESS)