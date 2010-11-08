#include "StdAfx.h"
#include ".\kernel32dll.h"

CKernel32Dll::CKernel32Dll(void)
:TDLLExports<void>("user32.dll")
{
}

CKernel32Dll::~CKernel32Dll(void)
{
}

DWORD CKernel32Dll::RegisterServiceProcess(DWORD dwProcessId, DWORD dwType)
{
	return (DWORD)TDLLExports<void>::CallStdFun("RegisterServicesProcess"
		, sizeof(DWORD) + sizeof(DWORD)
		, dwProcessId, dwType);
}