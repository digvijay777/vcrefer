#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <ntddk.h>
#ifdef __cplusplus
};
#endif
// ���潻��
#include "XRegmonCtrl.h"


#define PAGEDCODE			code_seg("PAGE")
#define LOCKEDCODE			code_seg()
#define INITCODE			code_seg("INIT")

#define PAGEDDATA			data_seg("PAGE")
#define LOCKEDDATA			data_seg()
#define INITDATA			data_seg("INIT")

#define arrayof(p)		( sizeof(p) / sizeof((p)[0]) )

typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT			pDevice;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


NTSTATUS CreateXRegmonDevice(PDRIVER_OBJECT pDriverObject);

void DDKXRegmonUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS DDKXRegmonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DDKXRegmonDispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DDKXRegmonDispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DDKXRegmonDispatchControl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DDKXRegmonDispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void LogWrite(const char* pFmt, ...);

#define XREGMON_DEVICE			L"\\Device\\DDKXRegmon"
#define XREGMON_LINKNAME		L"\\??\\XRegmon"
#define XREGMON_LOGFILE			L"\\??\\C:\\XMonitor.log"



// ͬ���¼�
#ifdef __cplusplus
extern "C"
{
#endif
	//
	// Definition for system call service table
	//
	typedef struct _SRVTABLE {
		PVOID           *ServiceTable;
		ULONG           LowCall;        
		ULONG           HiCall;
		PVOID		*ArgTable;
	} SRVTABLE, *PSRVTABLE;	
	// DDK ������C++
	extern PSRVTABLE	KeServiceDescriptorTable;	// ϵͳSSDT��
#ifdef __cplusplus
}
#endif
extern KEVENT		ghEventRead;				// ͬ�������¼�
extern KEVENT		ghEventWrite;				// ͬ����������
extern KMUTEX		ghMetuxSetValue;			// ����SetValueͬ��
extern KEVENT		ghEventStop;				// �Ƿ�ֹͣ�¼�
extern PSRVTABLE    ServiceTable;

	// ����ϵͳ����
#if defined(_ALPHA_)
#define SYSCALL(_function)  ServiceTable->ServiceTable[ (*(PULONG)_function)  & 0x0000FFFF ]
#else
#define SYSCALL(_function)  ServiceTable->ServiceTable[ *(PULONG)((PUCHAR)_function+1)]
#endif

// ��ʵ��д��ַ
typedef NTSTATUS (*fnRealRegSetValueKey)( IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName,
							   IN ULONG TitleIndex, IN ULONG Type, 
							   IN PVOID Data, IN ULONG DataSize );


void EnableServiceTable();
void DisableServiceTable();


#define SETSYSCALL(_fun, _setfun)		{ \
	EnableServiceTable(); \
	SYSCALL(_fun) = (PVOID)_setfun;\
	DisableServiceTable(); \
} \


//////////////////////////////////////////////////////////////////////////
NTSTATUS ObQueryNameString(PVOID Object, POBJECT_NAME_INFORMATION ObjectNameInfo, ULONG Length, PULONG ReturnLength);

typedef struct _rootkey {
	WCHAR                RootName[256];
	WCHAR                RootShort[32];
	ULONG               RootNameLen;
} ROOTKEY, *PROOTKEY;


