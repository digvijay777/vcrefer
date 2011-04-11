#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <ntddk.h>
#ifdef __cplusplus
};
#endif
// 界面交互
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



// 同步事件
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
	// DDK 不能用C++
	extern PSRVTABLE	KeServiceDescriptorTable;	// 系统SSDT表
#ifdef __cplusplus
}
#endif
extern KEVENT		ghEventRead;				// 同步处理事件
extern KEVENT		ghEventWrite;				// 同步错误数据
extern KMUTEX		ghMetuxSetValue;			// 设置SetValue同步
extern KEVENT		ghEventStop;				// 是否停止事件
extern PSRVTABLE    ServiceTable;

	// 定义系统调用
#if defined(_ALPHA_)
#define SYSCALL(_function)  ServiceTable->ServiceTable[ (*(PULONG)_function)  & 0x0000FFFF ]
#else
#define SYSCALL(_function)  ServiceTable->ServiceTable[ *(PULONG)((PUCHAR)_function+1)]
#endif

// 真实的写地址
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


