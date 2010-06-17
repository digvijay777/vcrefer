#include "StdAfx.h"
#include ".\wmiadapter.h"

WCHAR* CWMIAdapter::s_pNetAdapterState[] =
{
	L"未连接"				// 0
	, L"正在连接"			// 1
	, L"已连接"			// 2
	, L"正在断开连接"		// 3
	, L"硬件不存在"		// 4
	, L"硬件被禁止"		// 5
	, L"硬件工作不正常"	// 6
	, L"媒体已断开连接"	// 7
	, L"正在鉴定"			// 8
	, L"鉴定成功"			// 9
	, L"鉴定失败"			// 10
};

CWMIAdapter::CWMIAdapter(void)
{
	m_bConnect = FALSE;
}

CWMIAdapter::~CWMIAdapter(void)
{
}

BOOL CWMIAdapter::IsConnect()
{
	if(FALSE != m_bConnect)
		return TRUE;

	if(FALSE == CWMIBase::Initialize())
		return FALSE;
	if(FALSE == CWMIBase::ConnectServer("\\\\.\\root\\CIMV2"))
		return FALSE;

	m_bConnect = TRUE;
	return TRUE;
}

BOOL CWMIAdapter::InitAdapterInfo()
{
	m_vctAdapter.clear();

	if(FALSE == IsConnect())
		return FALSE;
// 	TCHAR*							pWQL		= "SELECT * FROM Win32_NetworkAdapter where AdapterTypeId=0";
	TCHAR*							pWQL		= "SELECT * FROM Win32_NetworkAdapter where NetConnectionID IS NOT NULL";
	CComPtr<IEnumWbemClassObject>	spObjects;

	if(FALSE == CWMIBase::ExecuteWQL(pWQL, &spObjects))
		return FALSE;

	CComPtr<IWbemClassObject>		spObject;
	HRESULT							hres;
	ULONG							nIndex		= 0;
	VARIANT							varRet;
	std::vector<ADAPTERINFO>::iterator	pAdapter	= NULL;
	ADAPTERINFO						info		= {0};
	do 
	{
		hres = spObjects->Next(WBEM_INFINITE, 1, &spObject, &nIndex);
		if(0 == nIndex)
			break;
		pAdapter = m_vctAdapter.insert(m_vctAdapter.end(), info);
		// DeviceID
		VariantInit(&varRet);
		spObject->Get(L"DeviceID", 0, &varRet, NULL, NULL);
		if(VT_NULL != varRet.vt)
		{
			VariantChangeType(&varRet, &varRet, 0, VT_UI4);
			pAdapter->nDeviceID = varRet.ulVal;
		}
		// MAC
		VariantInit(&varRet);
		spObject->Get(L"MACAddress", 0, &varRet, NULL, NULL);
		if(VT_NULL != varRet.vt)
		{
			VariantChangeType(&varRet, &varRet, 0, VT_BSTR);
			pAdapter->sMac = varRet.bstrVal;
		}
		// NetConnectionStatus
		VariantInit(&varRet);
		spObject->Get(L"NetConnectionStatus", 0, &varRet, NULL, NULL);
		if(VT_NULL != varRet.vt)
		{
			VariantChangeType(&varRet, &varRet, 0, VT_UI4);
			pAdapter->nState = varRet.ulVal;
		}
		// NetConnectionID
		VariantInit(&varRet);
		spObject->Get(L"NetConnectionID", 0, &varRet, NULL, NULL);
		if(VT_NULL != varRet.vt)
		{
			VariantChangeType(&varRet, &varRet, 0, VT_BSTR);
			pAdapter->sNetConnection = varRet.bstrVal;
		}
		// PNPDeviceID
		VariantInit(&varRet);
		spObject->Get(L"PNPDeviceID", 0, &varRet, NULL, NULL);
		if(VT_NULL != varRet.vt)
		{
			VariantChangeType(&varRet, &varRet, 0, VT_BSTR);
			pAdapter->sPNPDeviceID = varRet.bstrVal;
		}
		// 得到IP
		GetAdapterIPInfo(pAdapter->nDeviceID, *pAdapter);

		spObject.Release();
	} while (WBEM_S_NO_ERROR == hres);
	return TRUE;
}
BOOL CWMIAdapter::GetAdapterConfigObject(IWbemClassObject** pObject, int nIndex)
{
	TCHAR*							pWQL		= "SELECT * FROM Win32_NetworkAdapterConfiguration where Index = %d";
	CComPtr<IEnumWbemClassObject>	spObjects;
	TCHAR							szWQL[1024]	= {0};

	if(FALSE == IsConnect())
		return FALSE;

	_stprintf(szWQL, pWQL, nIndex);
	if(FALSE == CWMIBase::ExecuteWQL(szWQL, &spObjects))
		return FALSE;

	HRESULT							hres;
	ULONG							lRet		= 0;

	hres = spObjects->Next(WBEM_INFINITE, 1, pObject, &lRet);
	if(0 == nIndex)
		return FALSE;

	return TRUE;
}
BOOL CWMIAdapter::GetAdapterIPInfo(int nDeviceID, ADAPTERINFO& adapter)
{
	CComPtr<IWbemClassObject>		spObject;
	VARIANT							varRet;
	ADAPTERINFO						info		= {0};

	if(FALSE == GetAdapterConfigObject(&spObject, nDeviceID))
		return FALSE;
	// IPAddress
	VariantInit(&varRet);
	spObject->Get(L"IPAddress", 0, &varRet, NULL, NULL);
	if(VT_NULL != varRet.vt)
	{
		adapter.sIP = ConvertArrayToString(varRet);
	}
	// IPSubnet
	VariantInit(&varRet);
	spObject->Get(L"IPSubnet", 0, &varRet, NULL, NULL);
	if(VT_NULL != varRet.vt)
	{
		adapter.sSubIP = ConvertArrayToString(varRet);
	}
	// DefaultIPGateway
	VariantInit(&varRet);
	spObject->Get(L"DefaultIPGateway", 0, &varRet, NULL, NULL);
	if(VT_NULL != varRet.vt)
	{
		adapter.sGateway = ConvertArrayToString(varRet);
	}
	// DNSServerSearchOrder
	VariantInit(&varRet);
	spObject->Get(L"DNSServerSearchOrder", 0, &varRet, NULL, NULL);
	if(VT_NULL != varRet.vt)
	{
		adapter.sDNS = ConvertArrayToString(varRet);
	}

	spObject.Release();

	return TRUE;
}
std::wstring CWMIAdapter::ConvertArrayToString(VARIANT varArr, WCHAR* lpSplit /* = L */)
{
	std::wstring		strRet;

	if((VT_ARRAY|VT_BSTR) != varArr.vt)
		return L"";

	BSTR*	arr		= NULL;
	LONG	nLbound	= 0;
	LONG	nUBound = 0;

	SafeArrayGetLBound(varArr.parray, 1, &nLbound);
	SafeArrayGetLBound(varArr.parray, 1, &nUBound);
	SafeArrayAccessData(varArr.parray, (void **)&arr);
	for(int i = 0; i < (nUBound - nLbound + 1); i++)
	{
		if(0 != i)
			strRet += lpSplit;
		strRet += std::wstring(arr[i]);
	}
	SafeArrayUnaccessData(varArr.parray);

	return strRet;
}

VARIANT CWMIAdapter::ConvertStringToArray(std::wstring strArr, WCHAR* lpSplit /* = L */)
{
	VARIANT				varRet;
	WCHAR				szArr[1024]		= {0};
	WCHAR*				pTok			= NULL;
	std::vector<std::wstring> vctArr;

	VariantInit(&varRet);
	lstrcpynW(szArr, strArr.c_str(), sizeof(szArr));
	pTok = wcstok(szArr, lpSplit);
	for(; NULL != pTok; pTok = wcstok(NULL, lpSplit))
	{
		vctArr.push_back(pTok);
	}

	SAFEARRAYBOUND		bound;

	bound.cElements = vctArr.size();
	bound.lLbound = 0;
	varRet.vt = VT_BSTR|VT_ARRAY;
    varRet.parray = SafeArrayCreate(VT_BSTR, 1, &bound);
	for(LONG i = 0; i < vctArr.size(); i++)
		SafeArrayPutElement(varRet.parray, &i, ::SysAllocString(vctArr[i].c_str()));

	return varRet;
}

int CWMIAdapter::GetAdapterCount()
{
	return (int)m_vctAdapter.size();
}

BOOL CWMIAdapter::SetGateWay(int nDeviceID, LPWSTR lpGateWay, BOOL* pBRestart /* = NULL */)
{
	if(FALSE == IsConnect())
		return FALSE;

	TCHAR*			pWQL		= _T("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = true");
	CComPtr<IEnumWbemClassObject>		spEnumObject;
	HRESULT			hres		= 0;
	BOOL			bRet		= TRUE;
	VARIANT			varMethod;

	VariantInit(&varMethod);
	if(NULL != pBRestart)
		*pBRestart = FALSE;
	if(TRUE == CWMIBase::ExecuteWQL(pWQL, &spEnumObject))
	{
		CComPtr<IWbemClassObject>		spObject;
		ULONG							lRet			= 0;
		VARIANT							varRet;
		do 
		{
			spEnumObject->Next(WBEM_INFINITE, 1, &spObject, &lRet);
			if(0 == lRet)
				break;
			
			VariantInit(&varRet);
			spObject->Get(L"Index", 0, &varRet, NULL, NULL);
			VariantChangeType(&varRet, &varRet, 0, VT_UI4);

			VARIANT			varDefault;
			VARIANT			varMetric;

			VariantInit(&varDefault);
			VariantInit(&varMetric);
			varMetric.vt = VT_NULL;

			if(varRet.ulVal == nDeviceID)
			{
				varDefault = ConvertStringToArray(lpGateWay);
			}
			else
			{
				varDefault = ConvertStringToArray(L"");
			}
			
			CWMIBase::ExecMethod(spObject, L"SetGateways", &varDefault, &varMetric);

			VariantInit(&varDefault);

			spObject.Release();
		} while (TRUE);
	}
	if(VT_EMPTY == varMethod.vt)
		return FALSE;

	VariantChangeType(&varMethod, &varMethod, 0, VT_UI4);
	if((1 == varMethod.ulVal) && (NULL != pBRestart))
		*pBRestart = TRUE;
	if(1 < varMethod.ulVal)
	{
		SetLastErr("Set gateway failed: %d", varMethod.ulVal);
		return FALSE;
	}

	return TRUE;
}

BOOL CWMIAdapter::SetIP(int nDeviceID, LPWSTR lpIP, LPWSTR lpSubIP, BOOL* pBRestart /* = NULL */)
{
	CComPtr<IWbemClassObject>		spObject;

	if(NULL != pBRestart)
		*pBRestart = FALSE;
	if(FALSE == GetAdapterConfigObject(&spObject, nDeviceID))
		return FALSE;

	VARIANT		varIP		= ConvertStringToArray(lpIP);
	VARIANT		varSubIP	= ConvertStringToArray(lpSubIP);
	VARIANT		varRet;

	VariantInit(&varRet);
	varRet = CWMIBase::ExecMethod(spObject, L"EnableStatic", &varIP, &varSubIP);

	VariantInit(&varIP);
	VariantInit(&varSubIP);
	if(VT_EMPTY == varRet.vt)
		return FALSE;
	VariantChangeType(&varRet, &varRet, 0, VT_UI4);
    
	if((1 == varRet.ulVal) && (NULL != pBRestart))
		*pBRestart = TRUE;
	if(1 < varRet.ulVal)
	{
		SetLastErr("Set IP failed: %d", varRet.ulVal);
		return TRUE;
	}

	return FALSE;
}

BOOL CWMIAdapter::SetDNS(int nDeviceID, LPWSTR lpDNS, BOOL* pBRestart /* = NULL */)
{
	CComPtr<IWbemClassObject>		spObject;

	if(NULL != pBRestart)
		*pBRestart = FALSE;
	if(FALSE == GetAdapterConfigObject(&spObject, nDeviceID))
		return FALSE;

	VARIANT		varIP		= ConvertStringToArray(lpDNS);
	VARIANT		varNULL		= ConvertStringToArray(L"");
	VARIANT		varRet;

	VariantInit(&varRet);
	varRet = CWMIBase::ExecMethod(spObject, L"SetDNSServerSearchOrder", &varIP);

	VariantInit(&varIP);
	VariantInit(&varNULL);
	if(VT_EMPTY == varRet.vt)
		return FALSE;
	VariantChangeType(&varRet, &varRet, 0, VT_UI4);

	if((1 == varRet.ulVal) && (NULL != pBRestart))
		*pBRestart = TRUE;
	if(1 < varRet.ulVal)
	{
		SetLastErr("Set DNS failed: %d", varRet.ulVal);
		return TRUE;
	}

	return FALSE;
}