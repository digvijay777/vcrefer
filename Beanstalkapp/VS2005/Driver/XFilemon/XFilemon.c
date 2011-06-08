/*
 *	注册表监控
 */
#include "XFilemon.h"

//////////////////////////////////////////////////////////////////////////
// 入口函数
#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS		status = STATUS_SUCCESS;
	int				i = 0;

	DbgPrint("Enter DriverEntry\r\n");
	// 注册其他驱动调用函数入口
	for(; i < arrayof(pDriverObject->MajorFunction); i++)
	{
		pDriverObject->MajorFunction[i] = DDKXFilemonDispatchRoutine;
	}

	pDriverObject->DriverUnload = DDKXFilemonUnload;

	// 创建REGMON设备
	status = CreateXFilemonDevice(pDriverObject);
	
	return status;
}

// 创建设备
#pragma INITCODE
NTSTATUS CreateXFilemonDevice(PDRIVER_OBJECT pDriverObject)
{
	return STATUS_SUCCESS;
}
// 御载函数
#pragma PAGEDCODE
void DDKXFilemonUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	DbgPrint("Enter LogDDKUnload\r\n");
	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		
		pNexObj = pNexObj->NextDevice;
		IoDeleteDevice(pDevExt->pDevice);

	}
	DbgPrint("Leave LogDDKUnload\r\n");
}

// 默认派遣函数
// // #pragma PAGEDCODE
NTSTATUS DDKXFilemonDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	NTSTATUS status = STATUS_SUCCESS;
	//建立一个字符串数组与IRP类型对应起来
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

	UCHAR type = stack->MajorFunction;

	DbgPrint("Enter HelloDDKDispatchRoutin\r\n");

	if (type >= arrayof(irpname))
		DbgPrint(" - Unknown IRP, major type %X\r\n", type);
	else
		DbgPrint("\t%s\r\n", irpname[type]);


	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	DbgPrint("Leave HelloDDKDispatchRoutin\r\n");

	return status;
}
