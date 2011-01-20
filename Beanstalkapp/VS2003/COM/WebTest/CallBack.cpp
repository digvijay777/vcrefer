// CallBack.cpp : Implementation of CCallBack

#include "stdafx.h"
#include "CallBack.h"
#include ".\callback.h"

#pragma comment(lib, "comsupp.lib")
// CCallBack


STDMETHODIMP CCallBack::get_CallBack(VARIANT* pVal)
{
	*pVal = m_vtCallBack;

	return S_OK;
}

STDMETHODIMP CCallBack::put_CallBack(VARIANT newVal)
{
	m_vtCallBack = newVal;

	return S_OK;
}

STDMETHODIMP CCallBack::DoWork(void)
{

	if(NULL != m_hThread)
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 0))
			TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	if(VT_DISPATCH == m_vtCallBack.vt && NULL != m_vtCallBack.pdispVal)
	{
		CComPtr<IGlobalInterfaceTable>		spGlobal;

		spGlobal.CoCreateInstance(CLSID_StdGlobalInterfaceTable);
		m_vtCallBack.pdispVal->AddRef();
		spGlobal->RegisterInterfaceInGlobal(m_vtCallBack.pdispVal, __uuidof(IDispatch), &m_dwGlobalTable);
	}

	m_hThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);

	return S_OK;


// 	if(VT_DISPATCH != m_vtCallBack.vt)
// 		return S_OK;
// 
// 	DISPPARAMS		dispparams		= {0};
// 	_variant_t		vtRet;
// 	_bstr_t			str				= "这是COM回调";
// 	EXCEPINFO		exinfo			= {0};
// 	UINT			nArgErr			= -1;
// 	HRESULT			hres;
// 
// 	dispparams.cArgs = 2;
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 
// 	dispparams.rgvarg[0].vt = VT_I4;
// 	dispparams.rgvarg[0].lVal = 49;
// 	dispparams.rgvarg[1].vt = VT_BSTR;
// 	dispparams.rgvarg[1].bstrVal = str.GetBSTR();
// 	hres = m_vtCallBack.pdispVal->Invoke(DISPID_VALUE, IID_NULL, 0, DISPATCH_METHOD
// 		, &dispparams, &vtRet, &exinfo, &nArgErr);
// 	delete [] dispparams.rgvarg;
// 
// 	ATLASSERT(SUCCEEDED(hres));
	return S_OK;
}

DWORD CALLBACK CCallBack::WorkThread(LPVOID lpParameter)
{
	CCallBack*		pThis		= (CCallBack *)lpParameter;
	IDispatch*		pDispatch	= NULL;
    CComPtr<IGlobalInterfaceTable>		spGlobal;

	CoInitialize(NULL);
	spGlobal.CoCreateInstance(CLSID_StdGlobalInterfaceTable);
	spGlobal->GetInterfaceFromGlobal(pThis->m_dwGlobalTable, __uuidof(IDispatch), (void **)&pDispatch);
	spGlobal->RevokeInterfaceFromGlobal(pThis->m_dwGlobalTable);

	pThis->OnWork(pDispatch);

	pDispatch->Release();
	CloseHandle(pThis->m_hThread);
	pThis->m_hThread = NULL;
	return 0;
}

void CCallBack::OnWork(IDispatch* pDispatch)
{
	for(int i = 0; i < 100; i++)
	{
		Sleep(100);
		// 调用事件
		if(NULL != pDispatch)
		{
			DISPPARAMS		dispparams		= {0};
			WCHAR			szBuf[128]		= {0};
			_variant_t		vtRet;
			VARIANT			vtParam[2];
			_bstr_t			str				= "这是COM回调";
			EXCEPINFO		exinfo			= {0};
			UINT			nArgErr			= -1;

			dispparams.cArgs = sizeof(vtParam) / sizeof(VARIANT);
			dispparams.rgvarg = vtParam;
			wcscpy(szBuf, L"这是COM回调");
			
			dispparams.rgvarg[0].vt = VT_BSTR;
			dispparams.rgvarg[0].bstrVal = str.GetBSTR();
			dispparams.rgvarg[1].vt = VT_I4;
			dispparams.rgvarg[1].lVal = i+1;
			pDispatch->Invoke(DISPID_VALUE, IID_NULL, 0, DISPATCH_METHOD
				, &dispparams, &vtRet, &exinfo, &nArgErr);
		}
	}
}
STDMETHODIMP CCallBack::SetCallBack(VARIANT var)
{
	m_vtCallBack = var;

	return S_OK;
}
