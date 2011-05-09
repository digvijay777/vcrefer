#pragma once

extern "C"{
#include <ntddk.h>
#include <ntddkbd.h>
};
#include <stdio.h>
#include <stdarg.h>


#define PAGEDCODE			code_seg("PAGE")
#define LOCKEDCODE			code_seg()
#define INITCODE			code_seg("INIT")

#define PAGEDDATA			data_seg("PAGE")
#define LOCKEDDATA			data_seg()
#define INITDATA			data_seg("INIT")

#define arrayof(p)		( sizeof(p) / sizeof((p)[0]) )

// �չ
typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT			TopOfStack;
	bool					bDetach;
	LONG					lAttach;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


VOID KeyFilterUnload(IN PDRIVER_OBJECT Driver);
NTSTATUS KeyFilterRead( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp );
NTSTATUS KeyFilterDispatch( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp );
void LogWrite(const char* pFmt, ...);
NTSTATUS KeyReadComplate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);


