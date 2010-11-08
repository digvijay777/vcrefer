// DCOMTest.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "DCOMTest.h"
#include "Accctrl.h"
#include "Aclapi.h"
#include <ComDef.h>

#define NMASSERT		ATLASSERT
#include "acl.h"


#pragma comment(lib, "Advapi32.lib")

class CDCOMTestModule : public CAtlServiceModuleT< CDCOMTestModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_DCOMTestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DCOMTEST, "{4674D4A8-C320-4878-A48D-ED62D7208EC0}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}
	inline HRESULT RegisterAppId(bool bService = false)
	{
		HRESULT			hres;

		hres = CAtlServiceModuleT< CDCOMTestModule, IDS_SERVICENAME >::RegisterAppId(bService);

		if(FAILED(hres))
			return hres;

// 		GiveAppropriateAccessToCOMServer_426C80(GetAppIdT());
// 		return hres;
		// 更改权限
		TRUSTEE					tOwn		= {0};
		EXPLICIT_ACCESS			eAccess[2]	= {0};
		ULONG					nSize		= 0;
		PSECURITY_DESCRIPTOR	pSecurity	= NULL;
		DWORD					dwRes		= 0;

		// 指定权限
		memset( eAccess, 0, sizeof(eAccess) );
		eAccess[0].grfAccessPermissions = TRUE;
		eAccess[0].grfAccessMode = GRANT_ACCESS;
		eAccess[0].grfInheritance = FALSE;
		eAccess[0].Trustee.pMultipleTrustee = 0;
		eAccess[0].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
		eAccess[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
		eAccess[0].Trustee.TrusteeType = TRUSTEE_IS_USER;
		eAccess[0].Trustee.ptstrName = _T("SYSTEM");

		eAccess[1].grfAccessPermissions = TRUE;
		eAccess[1].grfAccessMode = GRANT_ACCESS;
		eAccess[1].grfInheritance = FALSE;
		eAccess[1].Trustee.pMultipleTrustee = 0;
		eAccess[1].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
		eAccess[1].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
		eAccess[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
		eAccess[1].Trustee.ptstrName = _T("Everyone");

		tOwn.pMultipleTrustee = 0;
		tOwn.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
		tOwn.TrusteeForm = TRUSTEE_IS_NAME;
		tOwn.TrusteeType = TRUSTEE_IS_GROUP;
		tOwn.ptstrName = "SYSTEM";

		// 构建权限二进制
		dwRes = BuildSecurityDescriptor(&tOwn, &tOwn, sizeof(eAccess) / sizeof(EXPLICIT_ACCESS), 
			eAccess, 0, NULL, NULL, &nSize, &pSecurity);
		if(ERROR_SUCCESS != dwRes)
		{
			ATLASSERT(FALSE);
			return hres;
		}

		// 写入注册表
		TCHAR			szSubPath[MAX_PATH]		= {0};

		_stprintf(szSubPath, _T("AppID\\%s"), GetAppIdT());

		// 执行权限
		SHSetValue(HKEY_CLASSES_ROOT, szSubPath, _T("AccessPermission"), REG_BINARY, pSecurity, nSize);
// 		// 启动权限
		SHSetValue(HKEY_CLASSES_ROOT, szSubPath, _T("LaunchPermission"), REG_BINARY, pSecurity, nSize);

		LocalFree(pSecurity);
		return hres;
	}
};

CDCOMTestModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

