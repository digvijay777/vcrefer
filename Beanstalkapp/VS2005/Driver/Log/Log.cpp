// #define NTSTRSAFE_LIB 
//#include <Ntstrsafe.h>
#include "log.h"

void LogWrite(const char* pFmt)
{
	OBJECT_ATTRIBUTES			obejectAttributes;
	IO_STATUS_BLOCK				iostatus;
	HANDLE						hfile;
	UNICODE_STRING				logFileUnicodeString;
	NTSTATUS					ntStatus;

	#define BUFFER_SIZE 1024

	RtlInitUnicodeString(&logFileUnicodeString, L"\\??\\C:\\logdriver.log");

	InitializeObjectAttributes(&obejectAttributes
		, &logFileUnicodeString
		, OBJ_CASE_INSENSITIVE
		, NULL
		, NULL);

	// 创建文件
	ntStatus = ZwCreateFile(&hfile
		, /*GENERIC_WRITE |*/ FILE_APPEND_DATA
		, &obejectAttributes
		, &iostatus
		, NULL
		, FILE_ATTRIBUTE_NORMAL
		, FILE_SHARE_READ
		, FILE_OPEN_IF
		, FILE_SYNCHRONOUS_IO_NONALERT
		, NULL
		, 0);
	if(NT_SUCCESS(ntStatus))
	{
// #define BUFFER_SIZE 1024
// 		PUCHAR		pBuffer = (PUCHAR)ExAllocatePool(PagedPool,BUFFER_SIZE);
// 		va_list		vaArr;
// 
// 		//RtlStrin
// 		va_start(vaArr, pFmt);
// 		vsnprintf(pFmt, BUFFER_SIZE, pFmt, vaArr);
// 		va_end(vaArr);
// 		//构造要填充的数据
// 		RtlFillMemory(pBuffer,BUFFER_SIZE,0xAA);
		
		ZwWriteFile(hfile, NULL, NULL, NULL, &iostatus, (void *)pFmt
			, strlen(pFmt), NULL, NULL);
// 		ExFreePool(pBuffer);

		ZwClose(hfile);
	}
}

#pragma INITCODE
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject
								, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS		status;

	LogWrite("Enter DriverEntry\r\n");

	// 注册其他驱动调用函数入口
	pDriverObject->DriverUnload = LogDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = LogDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = LogDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = LogDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = LogDDKDispatchRoutine;

	status = CreateDevice(pDriverObject);

	LogWrite("DriverEntry end\r\n");
	return status;
}

// 创建设备
#pragma INITCODE
NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS			status;
	PDEVICE_OBJECT		pDevObj;
	PDEVICE_EXTENSION	pDevExt;
	UNICODE_STRING		devName;
	UNICODE_STRING		linkName;

	RtlInitUnicodeString(&devName, L"\\Device\\LogDDKDevice");
	RtlInitUnicodeString(&linkName, L"\\??\\LogDDK");

	status = IoCreateDevice(pDriverObject
		, sizeof(DEVICE_EXTENSION)
		, &(UNICODE_STRING)devName
		, FILE_DEVICE_UNKNOWN
		, 0
		, TRUE
		, &pDevObj);

	if( !NT_SUCCESS(status) )
	{
		LogWrite("IoCreate Device Error: %d\r\n");
		return status;
	}

	
	pDevObj->Flags |= DO_BUFFERED_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	pDevExt->ustrSymLinkName = linkName;

	status = IoCreateSymbolicLink( &linkName, &devName );
	if( !NT_SUCCESS(status) )
	{
		IoDeleteDevice(pDevObj);
		return status;
	}

	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
void LogDDKUnload(PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT			pNexObj;

	pNexObj = pDriverObject->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		UNICODE_STRING			pLinkName	= pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		pNexObj = pNexObj->NextDevice;
		IoDeleteDevice(pDevExt->pDevice);
	}
	LogWrite("LogDDKUnload\r\n");
}

#pragma PAGEDCODE
NTSTATUS LogDDKDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	LogWrite("Enter LogDDKDispatchRoutine\r\n");
	NTSTATUS	status = STATUS_SUCCESS;

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	LogWrite("Leave LogDDKDispatchRoutine\r\n");
	return status;
}