// WebPlugins.cpp : Implementation of CWebPlugins
#include "stdafx.h"
#include "WebPlugins.h"


// CWebPlugins
STDMETHODIMP CWebPlugins::GetIDsOfNames(REFIID riid, OLECHAR FAR *FAR *rgszNames, unsigned int cNames, LCID lcid, DISPID FAR *rgDispId)
{
	if(S_OK == __super::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId))
		return S_OK;

	if(NULL != m_pPlugObject)
	{
		*rgDispId = m_pPlugObject->GetIDOfName(*rgszNames);
		return S_OK;
	}

	return E_INVALIDARG;
}
STDMETHODIMP CWebPlugins::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pDispParams
				  , VARIANT FAR *pVarResult, EXCEPINFO FAR *pExcepInfo, unsigned int FAR *puArgErr)
{
	if(S_OK == __super::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams
		, pVarResult, pExcepInfo, puArgErr))
		return S_OK;

	if(NULL == m_pPlugObject)
		return Error(L"未初始化实例");

	if( DISPATCH_METHOD == (DISPATCH_METHOD & wFlags) )
	{
		VARIANT*		pVal		= NULL;
		int				nSize		= 0;

		if(NULL != pDispParams)
		{
			nSize = pDispParams->cArgs;
			pVal = pDispParams->rgvarg;
		}

		if(FALSE != m_pPlugObject->CallMethod(dispIdMember, pVal, nSize, pVarResult))
			return S_OK;
		return Error(m_pPlugObject->GetLastError());
	}
	else if(DISPATCH_PROPERTYGET == wFlags)
	{
		if(FALSE != m_pPlugObject->GetProperty(dispIdMember, pVarResult))
			return S_OK;

		return Error(m_pPlugObject->GetLastError());
	}
	else if(DISPATCH_PROPERTYPUT == wFlags)
	{
		if(NULL == pDispParams || pDispParams->cArgs != 0)
			return Error(L"参数错误");

		if(FALSE != m_pPlugObject->SetProperty(dispIdMember, pDispParams->rgvarg[0]))
			return S_OK;

		return Error(m_pPlugObject->GetLastError());
	}
	else
	{
		return Error(L"未知的调用过程");
	}

	return Error(L"错误的调用过程");
}

LRESULT CWebPlugins::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(NULL != m_pPlugObject)
	{
		RECT		rect;

		GetClientRect(&rect);
		m_pPlugObject->SetWindow(m_hWnd, &rect);
	}
	return 0;
}

LRESULT CWebPlugins::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(NULL != m_pPlugObject)
	{
		RECT		rect;

		GetClientRect(&rect);
		m_pPlugObject->SetWindow(m_hWnd, &rect);
	}

	return 0;
}
