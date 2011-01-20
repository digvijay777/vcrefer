// MiniObject.cpp : Implementation of CMiniObject
#include "stdafx.h"
#include "MiniObject.h"
#include ".\miniobject.h"


// CMiniObject

STDMETHODIMP CMiniObject::put_CallBack(IDispatch* newVal)
{
	m_spCallBack = newVal;

	return S_OK;
}

STDMETHODIMP CMiniObject::Do(void)
{
	if(m_spCallBack == NULL)
		return S_OK;

	DISPPARAMS		dispparams		= {0};
	_variant_t		vtRet;
	_bstr_t			str				= "这是COM回调";
	EXCEPINFO		exinfo			= {0};
	UINT			nArgErr			= -1;
	HRESULT			hres;

	dispparams.cArgs = 2;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	dispparams.rgvarg[0].vt = VT_I4;
	dispparams.rgvarg[0].lVal = 50;
	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = str.GetBSTR();
	hres = m_spCallBack->Invoke(DISPID_VALUE, IID_NULL, 0, DISPATCH_METHOD
		, &dispparams, &vtRet, &exinfo, &nArgErr);
	delete [] dispparams.rgvarg;
	ATLASSERT(SUCCEEDED(hres));

	return S_OK;
}
