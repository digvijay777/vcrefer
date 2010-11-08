#pragma once
#include "../../../ExtClass/DLLExports.h"

class CKernel32Dll : public TDLLExports<void>
{
public:
	CKernel32Dll();
	~CKernel32Dll(void);
public:
	DWORD		RegisterServiceProcess(DWORD dwProcessId,DWORD dwType);
};
