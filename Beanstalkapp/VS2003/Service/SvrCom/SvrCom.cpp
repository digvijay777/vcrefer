// SvrCom.cpp : WinMain 的实现

#include "stdafx.h"
#include "resource.h"
#include "SvrCom.h"

class CSvrComModule : public CAtlServiceModuleT< CSvrComModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_SvrComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SVRCOM, "{73B46DCE-4BFC-4033-83EF-1687A445BA4D}")
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

CSvrComModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	DWORD			dwType		= 0;
	DWORD			dwSize		= 500;
	BYTE			pData[500]	= {0};
	CHAR			szDATA[1024] = {0};
// 	{
// 	val AccessPermission = b ''
// 		val LaunchPermission = b ''
//	}
// 	SHGetValue(HKEY_CLASSES_ROOT, "AppID\\{73B46DCE-4BFC-4033-83EF-1687A445BA4D}",
// 		"LaunchPermission", &dwType, pData, &dwSize);
// 
// 	for(int i = 0; i < dwSize; i++)
// 	{
// 		sprintf(szDATA + i*2, "%02x", pData[i]);
// 	}
    return _AtlModule.WinMain(nShowCmd);
}

