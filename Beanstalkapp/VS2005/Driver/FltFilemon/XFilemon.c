/*
 *	注册表监控
 */
#include "XFilemon.h"

FLT_PREOP_CALLBACK_STATUS FLTAPI NPPreCreate(__inout PFLT_CALLBACK_DATA Data
	, __in PCFLT_RELATED_OBJECTS FltObjects, __deref_out_opt PVOID *CompletionContext);
FLT_POSTOP_CALLBACK_STATUS FLTAPI NPPrePostCreate(__inout PFLT_CALLBACK_DATA Data
	, __in PCFLT_RELATED_OBJECTS FltObjects, __in_opt PVOID CompletionContext
	, __in FLT_POST_OPERATION_FLAGS Flags);
NTSTATUS NPUnload ( __in FLT_FILTER_UNLOAD_FLAGS Flags);
NTSTATUS SimRepInstanceSetup (__in PCFLT_RELATED_OBJECTS FltObjects
	, __in FLT_INSTANCE_SETUP_FLAGS Flags, __in DEVICE_TYPE VolumeDeviceType
	, __in FLT_FILESYSTEM_TYPE VolumeFilesystemType);

//////////////////////////////////////////////////////////////////////////
FLT_REGISTRATION				gFltReg			= {0};
PFLT_FILTER						gpFltFilter		= NULL;
FLT_OPERATION_REGISTRATION		gFltOR[2]		= {
	{IRP_MJ_CREATE, 0, NPPreCreate, NULL}
	, {IRP_MJ_OPERATION_END}
};


//////////////////////////////////////////////////////////////////////////
// 入口函数
#pragma INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS					status				= STATUS_SUCCESS;
	int							i					= 0;

	DbgPrint("Enter DriverEntry\r\n");
	// 注册其他驱动调用函数入口
// 	for(; i < arrayof(pDriverObject->MajorFunction); i++)
// 	{
// 		pDriverObject->MajorFunction[i] = DDKXFilemonDispatchRoutine;
// 	}
// 	pDriverObject->DriverUnload = DDKXFilemonUnload;
	// 过滤
	gFltReg.Size = sizeof(FLT_REGISTRATION);
	gFltReg.Version = FLT_REGISTRATION_VERSION;
	gFltReg.OperationRegistration = gFltOR;
	gFltReg.FilterUnloadCallback = NPUnload;
	gFltReg.InstanceSetupCallback = SimRepInstanceSetup;
	// 注册一个文件过滤
	status = FltRegisterFilter(pDriverObject, &gFltReg, &gpFltFilter);

	if(NT_SUCCESS(status))
	{
		KdPrint(("FltRegisterFilter success.\n"));
		status = FltStartFiltering(gpFltFilter);
		if(!NT_SUCCESS(status))
		{
			KdPrint(("FltStartFiltering failed.\n"));
			FltUnregisterFilter(gpFltFilter);
		}
	}
	else
	{
		if(STATUS_OBJECT_NAME_NOT_FOUND == status)
			KdPrint(("FltRegisterFilter failed: STATUS_OBJECT_NAME_NOT_FOUND\n"));
		else
			KdPrint(("FltRegisterFilter failed: 0x%X\n", status));
	}
	
	return status;
}

/************************************************************************/
/* 文件过滤处理函数                                                      */
/************************************************************************/
NTSTATUS NPUnload ( __in FLT_FILTER_UNLOAD_FLAGS Flags)
{
	UNREFERENCED_PARAMETER( Flags );
	PAGED_CODE();
	//注销此过滤器
	KdPrint(("Enter NPUnload\n"));
	FltUnregisterFilter( gpFltFilter );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
FLT_PREOP_CALLBACK_STATUS FLTAPI NPPreCreate(__inout PFLT_CALLBACK_DATA Data
											 , __in PCFLT_RELATED_OBJECTS FltObjects
											 , __deref_out_opt PVOID *CompletionContext)
{
	KdPrint(("Enter NPPreCreate\n"));
	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}
FLT_POSTOP_CALLBACK_STATUS FLTAPI NPPrePostCreate(__inout PFLT_CALLBACK_DATA Data
												  , __in PCFLT_RELATED_OBJECTS FltObjects
												  , __in_opt PVOID CompletionContext
												  , __in FLT_POST_OPERATION_FLAGS Flags)
{
	KdPrint(("Enter NPPrePostCreate\n"));
	return FLT_POSTOP_FINISHED_PROCESSING;
}

NTSTATUS SimRepInstanceSetup (
					 __in PCFLT_RELATED_OBJECTS FltObjects,
					 __in FLT_INSTANCE_SETUP_FLAGS Flags,
					 __in DEVICE_TYPE VolumeDeviceType,
					 __in FLT_FILESYSTEM_TYPE VolumeFilesystemType)
{

	UNREFERENCED_PARAMETER( FltObjects );
	UNREFERENCED_PARAMETER( Flags );
	UNREFERENCED_PARAMETER( VolumeDeviceType );
	UNREFERENCED_PARAMETER( VolumeFilesystemType );

	PAGED_CODE();


	if ( FlagOn( Flags, FLTFL_INSTANCE_SETUP_AUTOMATIC_ATTACHMENT ) ) {

		//
		//  Do not automatically attach to a volume.
		//

		KdPrint( ("[Simrep]: Instance setup skipped (Volume = %p, Instance = %p)\n",
			FltObjects->Volume,
			FltObjects->Instance) );

		return STATUS_SUCCESS;
	}


	//
	//  Attach on manual attachment.
	//

	KdPrint( ("[SimRep]: Instance setup started (Volume = %p, Instance = %p)\n",
		FltObjects->Volume, 
		FltObjects->Instance) );


	return STATUS_SUCCESS;
}
