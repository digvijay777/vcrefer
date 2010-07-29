// ProxyMon.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PreConnect.h"

HMODULE			g_hInstance		= NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	g_hInstance = (HMODULE)hModule;
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_preConnect.Attach();
		DisableThreadLibraryCalls(g_hInstance);
		break;
	case DLL_PROCESS_DETACH:
		g_preConnect.Detach();
		break;
	}
    return TRUE;
}

// 得到模块名
HINSTANCE	GetModuleInstance()
{
	if(NULL != g_hInstance)
		return g_hInstance;

	// 用模块查找方法
	MEMORY_BASIC_INFORMATION		basic		= {0};

	VirtualQuery(&g_hInstance, &basic, sizeof(basic));
	g_hInstance = (HMODULE)basic.AllocationBase;

	return g_hInstance;
}
