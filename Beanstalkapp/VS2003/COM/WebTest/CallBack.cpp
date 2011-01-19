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

	m_hThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);

	return S_OK;
}

DWORD CALLBACK CCallBack::WorkThread(LPVOID lpParameter)
{
	CCallBack*		pThis		= (CCallBack *)lpParameter;

	pThis->OnWork();
	CloseHandle(pThis->m_hThread);
	pThis->m_hThread = NULL;
	return 0;
}

void CCallBack::OnWork()
{
	for(int i = 0; i < 100; i++)
	{
		Sleep(100);
		// 调用事件
		if(VT_DISPATCH == m_vtCallBack.vt)
		{
			DISPPARAMS		dispparams		= {0};
			_variant_t		vtRet;
			_bstr_t			str				= "这是COM回调";
			EXCEPINFO		exinfo			= {0};
			UINT			nArgErr			= -1;

			dispparams.cArgs = 2;
			dispparams.rgvarg = new VARIANT[dispparams.cArgs];
			
			dispparams.rgvarg[0].vt = VT_I4;
			dispparams.rgvarg[0].lVal = i;
			dispparams.rgvarg[1].vt = VT_BSTR;
			dispparams.rgvarg[1].bstrVal = str.GetBSTR();
			m_vtCallBack.pdispVal->Invoke(-1, IID_NULL, 0, DISPATCH_METHOD
				, &dispparams, &vtRet, &exinfo, &nArgErr);
			delete [] dispparams.rgvarg;
		}
	}
}
STDMETHODIMP CCallBack::SetCallBack(VARIANT var)
{
	m_vtCallBack = var;

	return S_OK;
}
