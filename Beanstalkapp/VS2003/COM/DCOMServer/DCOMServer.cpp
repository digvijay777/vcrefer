// DCOMServer.cpp : WinMain 的实现

#include "stdafx.h"
#include "resource.h"
#include "DCOMServer.h"

class CDCOMServerModule : public CAtlServiceModuleT< CDCOMServerModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_DCOMServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DCOMSERVER, "{8C7B35A4-2698-4A14-A212-D6E5E763F6B4}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的 
		// 安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非空安全说明符。

		return S_OK;
	}
};

CDCOMServerModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

