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
#include "XFilemonCtrl.h"

#define SFLT_POOL_TAG 'tlFS' 

#define PAGEDCODE			code_seg("PAGE")
#define LOCKEDCODE			code_seg()
#define INITCODE			code_seg("INIT")

#define PAGEDDATA			data_seg("PAGE")
#define LOCKEDDATA			data_seg()
#define INITDATA			data_seg("INIT")

#define arrayof(p)		( sizeof(p) / sizeof((p)[0]) )

typedef struct _DEVICE_EXTENSION{
	PDEVICE_OBJECT			pDevice;
	PDEVICE_OBJECT			AttachedToDeviceObject;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
NTSTATUS CreateXFilemonDevice(PDRIVER_OBJECT pDriverObject);
void DDKXFilemonUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS DDKXFilemonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp);
/* 文件过滤特殊分发函数 */
NTSTATUS SfCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS SfFsControl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS SfCleanupClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);

NTSTATUS SfAttachDeviceToDeviceStack (IN PDEVICE_OBJECT SourceDevice, IN PDEVICE_OBJECT TargetDevice
									  , IN OUT PDEVICE_OBJECT *AttachedToDeviceObject);
typedef PDEVICE_OBJECT fnIoAttachDeviceToDeviceStack(IN PDEVICE_OBJECT  SourceDevice
													 , IN PDEVICE_OBJECT  TargetDevice);