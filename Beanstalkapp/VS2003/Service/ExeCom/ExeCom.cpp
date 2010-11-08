// ExeCom.cpp : WinMain µÄÊµÏÖ

#include "stdafx.h"
#include "resource.h"
#include "ExeCom.h"

class CExeComModule : public CAtlExeModuleT< CExeComModule >
{
public :
	DECLARE_LIBID(LIBID_ExeComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EXECOM, "{57B0AA6A-3022-42C0-AC96-E5F59158ED42}")
};

CExeComModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	
    return _AtlModule.WinMain(nShowCmd);
}

