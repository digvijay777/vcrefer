/*
 *	DCOM函数
 */
#define _WIN32_DCOM

#include <Windows.h>
#include <objbase.h>
#include <atlstr.h>

#include "DCOM.h"

// 创建远程DCOM接口
HRESULT DCOM::CreateInstance(REFCLSID rclsid, REFIID riid 
							 , LPCSTR lpSvrIP, LPCSTR lpUser, LPCSTR lpPasswd, void** ppUnknow)
{
	HRESULT			hRes		= E_FAIL;
	COSERVERINFO	svrInfo		= {0};
	COAUTHINFO		athn		= {0};
	COAUTHIDENTITY	author_id	= {0};
	BOOL			bRes		= FALSE;
	MULTI_QI		mqi[1]		= {0};
	CStringW		strSvr		= lpSvrIP;
	CStringW		strUser		= lpUser;
	CStringW		strPasswd	= lpPasswd;
	CStringW		strDomain	= "";

	// 设置COM安全属性
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE,
		RPC_C_IMP_LEVEL_ANONYMOUS, NULL, EOAC_NONE, NULL );

	author_id.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
	author_id.User = reinterpret_cast<USHORT *>(strUser.GetBuffer());
	author_id.UserLength = strUser.GetLength();
	author_id.Domain = reinterpret_cast<USHORT *>(strDomain.GetBuffer());
	author_id.DomainLength = strDomain.GetLength();
	author_id.Password = reinterpret_cast<USHORT *>(strPasswd.GetBuffer());
	author_id.PasswordLength = strPasswd.GetLength();
	
	athn.dwAuthnLevel = RPC_C_AUTHN_LEVEL_CONNECT;
	athn.dwAuthnSvc = RPC_C_AUTHN_WINNT;
	athn.dwAuthzSvc = RPC_C_AUTHZ_NONE;
	athn.dwCapabilities = EOAC_NONE;
	athn.dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
	athn.pAuthIdentityData = &author_id;
	athn.pwszServerPrincName = NULL;

	svrInfo.pwszName = strSvr.GetBuffer();
	svrInfo.pAuthInfo = &athn;

	mqi[0].pIID = &riid;

	hRes = CoCreateInstanceEx(rclsid, NULL, CLSCTX_REMOTE_SERVER, &svrInfo, 1, mqi);
	if(FAILED(hRes))
		return hRes;
	// 返回接口
	if(FAILED(mqi[0].hr))
		return hRes;

	// 设置接品权限
	hRes = CoSetProxyBlanket(mqi[0].pItf
		, athn.dwAuthnSvc
		, athn.dwAuthzSvc
		, NULL
		, athn.dwAuthnLevel
		, athn.dwImpersonationLevel
		, &author_id
		, EOAC_NONE);
	if(SUCCEEDED(hRes))
	{
		hRes = mqi[0].pItf->QueryInterface(riid, ppUnknow);
	}
	mqi[0].pItf->Release();

	return S_OK;
}