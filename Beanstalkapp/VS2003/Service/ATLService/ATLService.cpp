// ATLService.cpp : WinMain 的实现

#include "stdafx.h"
#include "resource.h"
#include "ATLService.h"

class CATLServiceModule : public CAtlServiceModuleT< CATLServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_ATLServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLSERVICE, "{B434FC52-7041-4A5A-A996-9E24B37B563B}")
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

CATLServiceModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

