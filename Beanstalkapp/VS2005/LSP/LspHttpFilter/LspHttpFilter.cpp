// LspHttpFilter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Installer/Installer.h"

HINSTANCE				gInst						= NULL;
TCHAR					gsProviderPath[MAX_PATH]	= {0};
/*
 *	DLL 入口函数
 */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if(DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		gInst = hModule;
		GetModuleFileName(hModule, gsProviderPath, MAX_PATH);
	}
    return TRUE;
}

/*
 *	注册LSP
 */
STDAPI DllRegisterServer(void)
{
	InstallProvider();
	return S_OK;
}

/*
 *	注销LSP
 */
STDAPI DllUnregisterServer(void)
{
	RemoveProvider();
	return S_OK;
}
