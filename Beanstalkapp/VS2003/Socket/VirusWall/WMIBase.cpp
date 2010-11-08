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
/* CWMIBase��                                                           */
/************************************************************************/
CWMIBase::CWMIBase()
{
	m_bInit = FALSE;
	m_spWbemLocator = NULL;
	m_spWbemServices = NULL;
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

// ��ʼ��
BOOL CWMIBase::Initialize()
{
	HRESULT		hres	= NULL;

	SetLastError(0);
	m_bstrErr = "";

	// ��ʼ��COM��
	hres = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hres))
	{
		SetLastError(hres);
		m_bstrErr = "CoInitializeEx Failed";
		return FALSE;
	}
	
	hres = ::CoInitializeSecurity(NULL,	-1, NULL, NULL, RPC_C_AUTHN_LEVEL_CONNECT, 
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL );
	if(FAILED(hres))
	{
		SetLastError(hres);
		m_bstrErr = "CoInitialzeEx Failed";
		::CoUninitialize();
		return FALSE;
	}

	m_bInit = TRUE;
}

/*
 *	���ӵ�������
 *
 *  ���ʱ��:2009-11-16
 */
BOOL CWMIBase::ConnectServer(LPCTSTR lpNamespace /* = NULL */, LPCTSTR lpUser /* = NULL */, LPCTSTR lpPassword /* = NULL */)
{
	HRESULT		hres	= S_OK;

	// ���IWbemLocator����
	if(NULL == m_spWbemLocator)
	{
		// ��ʼ��IWbemLocator����
		hres = ::CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, 
			(LPVOID *) &m_spWbemLocator);
		if(FAILED(hres))
		{
			SetLastError(hres);
			m_bstrErr = "CreateInstance IWbemLocator Failed";
			return FALSE;
		}
	}
	
	// ���ӷ�����
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
		m_bstrErr = "ConnectServer Failed";
		return FALSE;
	}

	// ����Ȩ��
	hres = ::CoSetProxyBlanket(m_spWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
		RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if(FAILED(hres))
	{
		SetLastError(hres);
		m_bstrErr = "CoSetProxyBlanket Failed";
		m_spWbemServices.Release();
		m_spWbemServices = NULL;
		return FALSE;
	}

	return TRUE;
}

/*
 *	ִ�в�ѯ
 *
 *  ���ʱ��:2009-11-16
 */
BOOL CWMIBase::ExecuteWQL(LPCTSTR lpWQL, IEnumWbemClassObject** ppEnum)
{
	HRESULT			hres;

	if(NULL == m_spWbemServices)
	{
		SetLastError(-1);
		m_bstrErr = "IWbemServices Object is NULL.";
		return FALSE;
	}

	// ��ʼ��ѯ
	_bstr_t			bstrWQL		= lpWQL;
	hres = m_spWbemServices->ExecQuery(_bstr_t("WQL"), bstrWQL, 
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, ppEnum);
	if(FAILED(hres))
	{
		SetLastError(hres);
		m_bstrErr = "ExecQuery Failed";
		return FALSE;
	}

	return TRUE;
}

// �õ�Service����
IWbemServices* CWMIBase::GetServices()
{
	return m_spWbemServices;
}

/*
 *	ִ�з���
 *
 *  ���ʱ��:2009-11-18
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
	
	// �õ������Ϣ
	hRes = pInstance->Get(L"__CLASS", 0, &varClass, &ciType, &lRes);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		m_bstrErr = "Get [__CLASS] failed.";
		return varRet;
	}
	hRes = pInstance->Get(L"__PATH", 0, &varPath, &ciType, &lRes);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		m_bstrErr = "Get [__PATH] failed.";
		return varRet;
	}

	// �õ���
	hRes = m_spWbemServices->GetObject(varClass.bstrVal, 0, NULL, &spClassObject, NULL);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		m_bstrErr = "Get class object failed.";
		return varRet;
	}
	
	CComPtr<IWbemClassObject>		spMethod;
	// �õ�����
	hRes = spClassObject->GetMethod(lpMethodName, 0, &spMethod, NULL);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		m_bstrErr = "Get method object failed.";
		return varRet;
	}

	CComPtr<IWbemClassObject>		spParameter;
	// �õ�����
	hRes = spMethod->SpawnInstance(0, &spParameter);
	if(FAILED(hRes))
	{
		SetLastError(hRes);
		m_bstrErr = "SpawnInstance object failed.";
		return varRet;
	}

	va_list			vaArrParam;
	SAFEARRAY*		pSafeParam;
	LONG			lBound		= 0;
	// ��������,����ֵ
	va_start(vaArrParam, lpMethodName);
	hRes = spParameter->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY, NULL, &pSafeParam);
	if(FAILED(hRes))
	{
		va_end(vaArrParam);
		SetLastError(hRes);
		m_bstrErr = "GetNames failed.";
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
	// ���÷���
	CComPtr<IWbemClassObject>		spOutParam;

	hRes = m_spWbemServices->ExecMethod(varPath.bstrVal, ::SysAllocString(lpMethodName), 0, NULL, spParameter, &spOutParam, NULL);
	if(SUCCEEDED(hRes))
	{
		hRes = spOutParam->Get(L"ReturnValue", 0, &varRet, &ciType, &lRes);
	}
	return varRet;
}
/************************************************************************/
/* CWMIClassObjects��                                                   */
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