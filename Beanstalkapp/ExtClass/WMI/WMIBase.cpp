#include "stdafx.h"
#include "WMIBase.h"

# pragma comment(lib, "wbemuuid.lib")

//////////////////////////////////////////////////////////////////////////
template <typename T>
bool operator==(int n, CComPtr<T> q)
{
	return (q == (void *)n);
}

template <typename T>
bool operator!=(int n, CComPtr<T> q)
{
	return (q != (void *)n);
}

/************************************************************************/
/* CWMIBase类                                                           */
/************************************************************************/
CWMIBase::CWMIBase()
{
	m_bInit = FALSE;
	m_spWbemLocator = NULL;
	m_spWbemServices = NULL;
	memset(m_szError, 0, sizeof(m_szError));
}

CWMIBase::~CWMIBase()
{
	if(NULL != m_spWbemLocator)
		m_spWbemLocator.Release();
	if(NULL != m_spWbemServices)
		m_spWbemServices.Release();
	if(FALSE != m_bInit)
		::CoUninitialize();
}

CHAR* CWMIBase::GetLastErr()
{
	return m_szError;
}

void CWMIBase::SetLastErr(LPCSTR lpFmt, ...)
{
	va_list		vaErr;

	va_start(vaErr, lpFmt);
	_vsnprintf(m_szError, sizeof(m_szError), lpFmt, vaErr);
	va_end(vaErr);
}

// 初始化
BOOL CWMIBase::Initialize()
{
	HRESULT		hres	= NULL;

	SetLastError(0);
	memset(m_szError, 0, sizeof(m_szError));

	if(FALSE != m_bInit)
		return TRUE;

	// 初始化COM库
	hres = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hres))
	{
		SetLastError(hres);
		SetLastErr("CoInitializeEx failed: %d", hres);
		return FALSE;
	}
	
	hres = ::CoInitializeSecurity(NULL,	-1, NULL, NULL, RPC_C_AUTHN_LEVEL_CONNECT, 
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL );
	if(FAILED(hres))
	{
		SetLastError(hres);
		SetLastErr("CoInitializeSecurity failed: %d", hres);
		::CoUninitialize();
		return FALSE;
	}

	m_bInit = TRUE;
	return TRUE;
}

/*
 *	连接到服务器
 *
 *  添加时间:2009-11-16
 */
BOOL CWMIBase::ConnectServer(LPCTSTR lpNamespace /* = NULL */, LPCTSTR lpUser /* = NULL */, LPCTSTR lpPassword /* = NULL */)
{
	HRESULT		hres	= S_OK;

	// 检测IWbemLocator对象
	if(NULL == m_spWbemLocator)
	{
		// 初始化IWbemLocator对象
		hres = ::CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, 
			(LPVOID *) &m_spWbemLocator);
		if(FAILED(hres))
		{
			SetLastError(hres);
			SetLastErr("Create interface IWbemLocator failed: %d", hres);
			return FALSE;
		}
	}
	
	// 连接服务器
	if(NULL != m_spWbemServices)
		m_spWbemServices.Release();
	m_spWbemServices = NULL;

	_bstr_t			bstrNamespace;
	_bstr_t			bstrUser;
	_bstr_t			bstrPasswd;
	BSTR			pUser			= NULL;
	BSTR			pPasswd			= NULL;
	
	if(NULL == lpNamespace)
		bstrNamespace = "\\\\.\\root\\default";
	else
		bstrNamespace = lpNamespace;

	if(NULL != lpUser)
	{
		bstrUser = lpUser;
		pUser = bstrUser;
	}
	if(NULL != lpPassword)
	{
		bstrPasswd = lpPassword;
		pPasswd = bstrPasswd;
	}
	hres = m_spWbemLocator->ConnectServer(bstrNamespace, pUser, pPasswd, NULL, 0, NULL, NULL, &m_spWbemServices);
	if(FAILED(hres))
	{
		SetLastError(hres);
		SetLastErr("ConnectServer %S failed: %d", (WCHAR *)bstrNamespace, hres);
		return FALSE;
	}

	// 设置权限
	hres = ::CoSetProxyBlanket(m_spWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
		RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if(FAILED(hres))
	{
		SetLastError(hres);
		SetLastErr("CoSetProxyBlanket %S failed: %d", (WCHAR *)bstrNamespace, hres);
		m_spWbemServices.Release();
		m_spWbemServices = NULL;
		return FALSE;
	}

	return TRUE;
}

/*
 *	执行查询
 *
 *  添加时间:2009-11-16
 */
BOOL CWMIBase::ExecuteWQL(LPCTSTR lpWQL, IEnumWbemClassObject** ppEnum)
{
	HRESULT			hres;

	if(NULL == m_spWbemServices)
	{
		SetLastError(-1);
		SetLastErr("IWbemServices Object is NULL.");
		return FALSE;
	}

	// 开始查询
	_bstr_t			bstrWQL		= lpWQL;
	hres = m_spWbemServices->ExecQuery(_bstr_t("WQL"), bstrWQL, 
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, ppEnum);
	if(FAILED(hres))
	{
		SetLastError(hres);
		SetLastErr("ExecQuery %S failed: %d", (WCHAR *)bstrWQL, hres);
		return FALSE;
	}

	return TRUE;
}

// 得到Service对象
IWbemServices* CWMIBase::GetServices()
{
	return m_spWbemServices;
}

/*
 *	执行方法
 *
 *  添加时间:2009-11-18
 */
VARIANT CWMIBase::ExecMethod(IWbemClassObject* pInstance, LPCWSTR lpMethodName, ...)
{
	VARIANT			varRet	= {0};

	varRet.vt = VT_EMPTY;
	if(NULL == m_spWbemServices)
	{
		return varRet;
	}

	HRESULT			hRes;
	CComVariant		varClass;
	CComVariant		varPath;
	CIMTYPE			ciType;
	LONG			lRes;
	CComPtr<IWbemClassObject>		spClassObject;
	
	// 得到相关信息
	hRes = pInstance->Get(L"__CLASS", 0, &varClass, &ciType, &lRes);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		SetLastErr("ExecMethod %S:Get [__CLASS] failed %d", lpMethodName, hRes);
		return varRet;
	}
	hRes = pInstance->Get(L"__PATH", 0, &varPath, &ciType, &lRes);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		SetLastErr("ExecMethod %S:Get [__PATH] failed %d", lpMethodName, hRes);
		return varRet;
	}

	// 得到类
	hRes = m_spWbemServices->GetObject(varClass.bstrVal, 0, NULL, &spClassObject, NULL);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		SetLastErr("ExecMethod %S:Get object failed %d", lpMethodName, hRes);
		return varRet;
	}
	
	CComPtr<IWbemClassObject>		spMethod;
	// 得到方法
	hRes = spClassObject->GetMethod(lpMethodName, 0, &spMethod, NULL);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		SetLastErr("ExecMethod %S:Get method failed %d", lpMethodName, hRes);
		return varRet;
	}

	CComPtr<IWbemClassObject>		spParameter;
	// 得到参数
	hRes = spMethod->SpawnInstance(0, &spParameter);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		SetLastErr("ExecMethod %S:SpawnInstance failed %d", lpMethodName, hRes);
		return varRet;
	}

	va_list			vaArrParam;
	SAFEARRAY*		pSafeParam;
	LONG			lBound		= 0;
	// 分析参数,并负值
	va_start(vaArrParam, lpMethodName);
	hRes = spParameter->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY, NULL, &pSafeParam);
	if(FAILED(hRes))
	{
		va_end(vaArrParam);
		SetLastError(hRes);
		SetLastErr("ExecMethod %S:GetNames failed %d", lpMethodName, hRes);
		return varRet;
	}
	do 
	{
		BSTR			bstrName;
		hRes = SafeArrayGetElement(pSafeParam, &lBound, &bstrName);
		if(FAILED(hRes))
			break;
		lBound++;

		VARIANT*		pVar = va_arg(vaArrParam, VARIANT*);
		hRes = spParameter->Put(bstrName, 0, pVar, 0);
		if(FAILED(hRes))
			break;
	} while (1);
	SafeArrayDestroy(pSafeParam);
	va_end(vaArrParam);
	// 调用方法
	CComPtr<IWbemClassObject>		spOutParam;

	hRes = m_spWbemServices->ExecMethod(varPath.bstrVal, ::SysAllocString(lpMethodName), 0, NULL, spParameter, &spOutParam, NULL);
	if(SUCCEEDED(hRes))
	{
		hRes = spOutParam->Get(L"ReturnValue", 0, &varRet, &ciType, &lRes);
	}
	return varRet;
}
/************************************************************************/
/* CWMIClassObjects类                                                   */
/************************************************************************/

CWMIClassObjects::CWMIClassObjects()
{
	m_pCurWbemObject = NULL;
	m_spEnumWbem = NULL;
}

CWMIClassObjects::~CWMIClassObjects()
{
	if(NULL != m_spEnumWbem)
		m_spEnumWbem.Release();
}

IEnumWbemClassObject** CWMIClassObjects::operator &()
{
	return &m_spEnumWbem;
}

BOOL CWMIClassObjects::MoveFirst()
{
	if(NULL == m_spEnumWbem)
	{
		return FALSE;
	}

	if(NULL == m_pCurWbemObject)
		m_spEnumWbem->Reset();

}