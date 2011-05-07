#include "KeyFilter.h"

#pragma INITCODE
extern "C" NTSTATUS DriverEntry( IN PDRIVER_OBJECT  DriverObject, IN PUNICODE_STRING RegistryPath )
{
	NTSTATUS		status		= STATUS_SUCCESS;
	// 注册派遣函数
	for(int i = 0; i < arrayof(DriverObject->MajorFunction); i++)
	{
		DriverObject->MajorFunction[i] = KeyFilterDispatch;
	}
	DriverObject->MajorFunction[IRP_MJ_READ] = KeyFilterRead;
	DriverObject->DriverUnload = KeyFilterUnload;
	// 创建设备
	PDEVICE_OBJECT	pDevice		= NULL;

	status = IoCreateDevice(DriverObject
		, sizeof(DEVICE_EXTENSION)
		, NULL
		, FILE_DEVICE_UNKNOWN
		, 0
		, TRUE
		, &pDevice);
	if(!NT_SUCCESS(status))
	{
		LogWrite("Create Device Error:%d\r\n", status);
		return status;
	}
	// 挂接键盘
	PDEVICE_EXTENSION		pDevExt;
	UNICODE_STRING			devKeybord;

	pDevExt = (PDEVICE_EXTENSION)pDevice->DeviceExtension;
	RtlInitUnicodeString(&devKeybord, L"\\Device\\KeyboardClass0");
	status = IoAttachDevice(pDevice, &devKeybord, &pDevExt->TopOfStack);
	if(!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevice);
		LogWrite("Attach Device Error: %d\r\b", status);
		return status;
	}
	// 设置设备属性
	pDevice->DeviceType = pDevExt->TopOfStack->DeviceType;
	pDevice->Characteristics = pDevExt->TopOfStack->Characteristics;
	pDevice->Flags &= ~DO_DEVICE_INITIALIZING;
	pDevice->Flags |= (pDevExt->TopOfStack->Flags & (DO_BUFFERED_IO | DO_DIRECT_IO));

	LogWrite("KeyFilter Attach Success.\r\n");
	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
VOID KeyFilterUnload(IN PDRIVER_OBJECT Driver)
{
	PDEVICE_OBJECT			pNexObj;

	pNexObj = Driver->DeviceObject;
	while(NULL != pNexObj)
	{
		PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)pNexObj->DeviceExtension;
		PDEVICE_OBJECT			pDelDev		= pNexObj;
		
		pNexObj = pNexObj->NextDevice;
		if(NULL != pDevExt->TopOfStack)
			IoDetachDevice(pDevExt->TopOfStack);
		IoDeleteDevice(pDelDev);
		LogWrite("KeyFilter Detach Success.\r\n");
	}
}

#pragma PAGEDCODE
NTSTATUS KeyFilterRead( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp )
{
	PDEVICE_EXTENSION		pDevExt		= (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;

	// 复制IRP
	IoCopyCurrentIrpStackLocationToNext(Irp);
	// 设置完成例程
	IoSetCompletionRoutine(Irp, KeyReadComplate, DeviceObject, TRUE, TRUE, TRUE);

	return IoCallDriver(pDevExt->TopOfStack, Irp);
}

#pragma PAGEDCODE
NTSTATUS KeyReadComplate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PIO_STACK_LOCATION		stack		= IoGetCurrentIrpStackLocation(Irp);
	PKEYBOARD_INPUT_DATA	keydata		= NULL;
	int						numkey		= 0;

	if(NT_SUCCESS(Irp->IoStatus.Status))
	{
		keydata = (PKEYBOARD_INPUT_DATA)Irp->AssociatedIrp.SystemBuffer;
		numkey = Irp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);
		// 记录所键入的KEY
		//LogWrite("[KD]:");
		for(int i = 0; i < numkey; i++)
		{
			if(0x1E == keydata[i].MakeCode)
				keydata[i].MakeCode = 0x30;
			else if(0x30 == keydata[i].MakeCode)
				keydata[i].MakeCode = 0x1E;
			
			//DbgPrint("\td[c]");
			//LogWrite("\t%d[%c]", keydata[i].MakeCode, keydata[i].MakeCode);
		}
 		//LogWrite("\r\n");
	}

	if(Irp->PendingReturned)
	{
		IoMarkIrpPending(Irp);
	}

	return Irp->IoStatus.Status;
}

#pragma PAGEDCODE
NTSTATUS KeyFilterDispatch( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp )
{
	IoSkipCurrentIrpStackLocation(Irp);

	return IoCallDriver(((PDEVICE_EXTENSION) DeviceObject->DeviceExtension)->TopOfStack, Irp);
}

#pragma LOCKEDCODE
void LogWrite(const char* pFmt, ...)
{
	OBJECT_ATTRIBUTES			obejectAttributes;
	IO_STATUS_BLOCK				iostatus;
	HANDLE						hfile;
	UNICODE_STRING				logFileUnicodeString;
	NTSTATUS					ntStatus;
	PUCHAR						pstr;

	RtlInitUnicodeString(&logFileUnicodeString, L"\\??\\C:\\Keyborad.log");

	InitializeObjectAttributes(&obejectAttributes
		, &logFileUnicodeString
		, OBJ_CASE_INSENSITIVE
		, NULL
		, NULL);
	pstr = (PUCHAR)ExAllocatePool(NonPagedPool, 1024);

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
		va_list		varArr;

		va_start(varArr, pFmt);
		_vsnprintf((char *)pstr, 1024, pFmt, varArr);
		va_end(varArr);
		ZwWriteFile(hfile, NULL, NULL, NULL, &iostatus, (void *)pstr
			, strlen((char *)pstr), NULL, NULL);
		ZwClose(hfile);
	}

	ExFreePool(pstr);
}