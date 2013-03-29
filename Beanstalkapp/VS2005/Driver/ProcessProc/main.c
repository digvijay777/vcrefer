/*************************************************
    File name: 这是一个进程保护测试
    Author: yj	Version: 	Date:3/29/2013
    History: 修改历史记录（包括Date,Author,Modification）
*************************************************/
#include <wdm.h>

ULONG		ProcessNameOffset		= 0;
PVOID		g_hProcCreateHandler	= NULL;

void DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS RegisteCallbackFunction();

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING Register)
{
	PEPROCESS		epro	= PsGetCurrentProcess();
	PVOID			p		= DriverObject->DriverSection;
// 	PLDR_DATA_TABLE_ENTRY pLdrEntry=(PLDR_DATA_TABLE_ENTRY)pDrvObj->DriverSection;
// 	pLdrEntry->Flags |=0x20;
	ULONG*			pFlag	= (ULONG *)((CHAR *)p + 0x68);

	// 获取名称标识
	for(; ProcessNameOffset < 1024; ProcessNameOffset++)
	{
		if(_strnicmp("system", (char *)epro + ProcessNameOffset, 6) == 0)
		{
			break;
		}
	}
	
	// 设置签名位
	*pFlag |= 0x20;

	DriverObject->DriverUnload = DriverUnload;
	if(STATUS_SUCCESS == RegisteCallbackFunction())
	{
		return STATUS_SUCCESS;
	}

	return STATUS_UNSUCCESSFUL;
}

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if(NULL != g_hProcCreateHandler)
	{
		ObUnRegisterCallbacks(g_hProcCreateHandler);
	}
}

#define PROCESS_QUERY_INFORMATION (0x0400)
#define PROCESS_TERMINATE         (0x0001) 

OB_PREOP_CALLBACK_STATUS	PreProcCreateRoutine(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	//PEPROCESS		epro	= PsGetCurrentProcess();
	PEPROCESS		epro	= (PEPROCESS)OperationInformation->Object;
	NTSTATUS		ntStatus;

	if(*PsProcessType != OperationInformation->ObjectType
		|| 0 != OperationInformation->KernelHandle)
	{
		return OB_PREOP_SUCCESS; 
	}

	if(_strnicmp((char*)epro + ProcessNameOffset, "notepad.exe", 11) == 0)
	{
		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess
			&= (~PROCESS_TERMINATE);
	}

	return OB_PREOP_SUCCESS;
}

void PostProcCreateRoutine(
						   PVOID RegistrationContext,
						   POB_POST_OPERATION_INFORMATION OperationInformation)
{

}

typedef struct _OB_REG_CONTEXT {
	__in USHORT                     Version;
	__in UNICODE_STRING             Altitude;    
	__in USHORT                     ulIndex;    
} REG_CONTEXT, *PREG_CONTEXT;

NTSTATUS RegisteCallbackFunction()
{
	NTSTATUS			ntStatus		= STATUS_SUCCESS;
	UNICODE_STRING		Altitude		= RTL_CONSTANT_STRING(L"320100");
	USHORT				filterVersion	= ObGetFilterVersion();
	USHORT				registrationCount = 2;
	OB_OPERATION_REGISTRATION	RegisterOperation	= {0};
	OB_CALLBACK_REGISTRATION	RegisterCallBack	= {0};
	REG_CONTEXT					RegistrationContext	= {0};

	RegistrationContext.ulIndex = 1;
	RegistrationContext.Version = 120;
	if(OB_FLT_REGISTRATION_VERSION != filterVersion)
	{
		return STATUS_UNSUCCESSFUL;
	}

	RegisterOperation.ObjectType = PsProcessType;
	RegisterOperation.Operations = OB_OPERATION_HANDLE_CREATE;
	RegisterOperation.PreOperation = PreProcCreateRoutine;
	RegisterOperation.PostOperation = PostProcCreateRoutine;

	RegisterCallBack.Version = OB_FLT_REGISTRATION_VERSION;
	RegisterCallBack.OperationRegistrationCount = 1/*registrationCount*/;
	RegisterCallBack.Altitude = Altitude;
	RegisterCallBack.RegistrationContext = NULL/*&RegistrationContext*/;
	RegisterCallBack.OperationRegistration = &RegisterOperation;

	ntStatus = ObRegisterCallbacks(&RegisterCallBack, &g_hProcCreateHandler);
	return ntStatus;
}