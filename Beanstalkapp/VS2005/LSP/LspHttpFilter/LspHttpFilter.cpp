// LspHttpFilter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Installer/Installer.h"

HINSTANCE				gInst						= NULL;
TCHAR					gsProviderPath[MAX_PATH]	= {0};
/*
 *	DLL ��ں���
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
 *	ע��LSP
 */
STDAPI DllRegisterServer(void)
{
	InstallProvider();
	return S_OK;
}

/*
 *	ע��LSP
 */
STDAPI DllUnregisterServer(void)
{
	RemoveProvider();
	return S_OK;
}
