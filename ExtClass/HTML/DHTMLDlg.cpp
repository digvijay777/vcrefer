#include "stdafx.h"
#include "DHTMLDlg.h"

//////////////////////////////////////////////////////////////////////////
CDHtmlDialog*		DHTML::g_pDHTMLDialog		= NULL;

//////////////////////////////////////////////////////////////////////////
// 设置元素属性[DHTML的基本函数]
HRESULT DHTML::SetElementProperty(LPCTSTR lpID, DISPID dispid, VARIANT *pVar)
{
	if(FALSE == DHTML::CheckDHTMLDialogPointer(DHTML::g_pDHTMLDialog))
		return E_INVALIDARG;

	DHTML::g_pDHTMLDialog->SetElementProperty(lpID, dispid, pVar);
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
// 得到元素属性[DHTML的基本函数]
VARIANT	DHTML::GetElementProperty(LPCTSTR lpID, DISPID dispid)
{
	if(FALSE == DHTML::CheckDHTMLDialogPointer(DHTML::g_pDHTMLDialog))
		return VARIANT();

	return DHTML::g_pDHTMLDialog->GetElementProperty(lpID, dispid);
}
//////////////////////////////////////////////////////////////////////////
// 设置元素的InnerHTML
HRESULT	DHTML::SetHTMLInnerHTML(LPCTSTR lpID, LPCTSTR szHtml)
{
	VARIANT		vt;
	HRESULT		hr;

	VariantInit(&vt);
	vt.vt = VT_BSTR;
	vt.bstrVal = ::SysAllocString(CStringW(szHtml).GetBuffer());
	hr = DHTML::SetElementProperty(lpID, DISPID_IHTMLELEMENT_INNERHTML, &vt);
	::SysFreeString(vt.bstrVal);
	return hr;
}
//////////////////////////////////////////////////////////////////////////
// 得到元素的InnerHTML
CString	DHTML::GetHTMLInnerHTML(LPCTSTR lpID)
{
	CString		rtStr	= _T("");
	VARIANT		var;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLELEMENT_INNERHTML);
	if(VT_BSTR == var.vt)
		rtStr = var.bstrVal;
	return rtStr;
}
//////////////////////////////////////////////////////////////////////////
// 设置元素的InnerHTML
HRESULT DHTML::SetHTMLInnerText(LPCTSTR lpID, LPCTSTR szText)
{
	VARIANT		vt;
	HRESULT		hr;

	VariantInit(&vt);
	vt.vt = VT_BSTR;
	vt.bstrVal = ::SysAllocString(CStringW(szText).GetBuffer());
	hr = DHTML::SetElementProperty(lpID, DISPID_IHTMLELEMENT_INNERTEXT, &vt);
	::SysFreeString(vt.bstrVal);
	return hr;
}
//////////////////////////////////////////////////////////////////////////
// 得到元素的InnerText
CString DHTML::GetHTMLInnerText(LPCTSTR lpID)
{
	CString		rtStr	= _T("");
	VARIANT		var;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLELEMENT_INNERHTML);
	if(VT_BSTR == var.vt)
		rtStr = var.bstrVal;
	return rtStr;
}
//////////////////////////////////////////////////////////////////////////
// 得到ActiveX控件的接口
HRESULT	DHTML::GetHTMLElementInterface(LPCTSTR lpID, REFIID riid, void** ppvObject)
{
	if(FALSE == DHTML::CheckDHTMLDialogPointer(DHTML::g_pDHTMLDialog))
		return E_INVALIDARG;

	return DHTML::g_pDHTMLDialog->GetElementInterface(lpID, riid, ppvObject);}

//////////////////////////////////////////////////////////////////////////
// 设置Input元素的可用性
HRESULT	DHTML::SetHTMLInputDisabled(LPCTSTR lpID, BOOL bDisbled)
{
	VARIANT		var;

	VariantInit(&var);
	var.vt = VT_BOOL;
	var.boolVal = (TRUE == bDisbled)?VARIANT_TRUE:VARIANT_FALSE;
	return DHTML::SetElementProperty(lpID, DISPID_IHTMLINPUTELEMENT_DISABLED, &var);
}
//////////////////////////////////////////////////////////////////////////
// 得到Input元素的可用性
BOOL DHTML::GetHTMLInputDisabled(LPCTSTR lpID)
{
	VARIANT			var;
	BOOL			bRet	= FALSE;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLINPUTELEMENT_DISABLED);
	if(VT_BOOL == var.vt)
		bRet = (VARIANT_TRUE == var.boolVal)?TRUE:FALSE;
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// 设置Input元素的先择状态
HRESULT DHTML::SetHTMLInputChecked(LPCTSTR lpID, BOOL bChecked)
{
	VARIANT		var;

	VariantInit(&var);
	var.vt = VT_BOOL;
	var.boolVal = (TRUE == bChecked)?VARIANT_TRUE:VARIANT_FALSE;
	return DHTML::SetElementProperty(lpID, DISPID_IHTMLINPUTELEMENT_CHECKED, &var);
}
//////////////////////////////////////////////////////////////////////////
// 得到Input元素的选择状态
BOOL DHTML::GetHTMLInputChecked(LPCTSTR lpID)
{
	VARIANT		var;
	BOOL		bRet	= FALSE;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLINPUTELEMENT_CHECKED);
	if(VT_BOOL == var.vt)
		bRet = (VARIANT_TRUE == var.boolVal)?TRUE:FALSE;
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// 得到Input的Value
HRESULT	DHTML::SetHTMLInputValue(LPCTSTR lpID, LPCTSTR szValue)
{
	VARIANT			var;
	HRESULT			hr;

	VariantInit(&var);
	var.vt = VT_BSTR;
	var.bstrVal = ::SysAllocString(CStringW(szValue).GetBuffer());
	hr = DHTML::SetElementProperty(lpID, DISPID_IHTMLINPUTELEMENT_VALUE, &var);
	::SysFreeString(var.bstrVal);
	return hr;
}
//////////////////////////////////////////////////////////////////////////
// 得到Input元素的Value
CString	DHTML::GetHTMLInputValue(LPCTSTR lpID)
{
	CString		rtStr	 = _T("");
	VARIANT		var;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLINPUTELEMENT_VALUE);
	if(VT_BSTR == var.vt)
		rtStr = var.bstrVal;
	return rtStr;
}
//////////////////////////////////////////////////////////////////////////
// 设置HTML的Onclick事件
HRESULT	DHTML::SetHTMLOnClicked(LPCTSTR lpID, VARIANT* pEvent)
{
	return DHTML::SetElementProperty(lpID, DISPID_IHTMLELEMENT_ONCLICK, pEvent);
}
//////////////////////////////////////////////////////////////////////////
// 得到HTML元素的OnClick事件的		
VARIANT	DHTML::GetHTMLOnClicked(LPCTSTR lpID)
{
	return DHTML::GetElementProperty(lpID, DISPID_IHTMLELEMENT_ONCLICK);
}
//////////////////////////////////////////////////////////////////////////
// 拷贝OnClick事件
HRESULT DHTML::CopyHTMLOnClicked(LPCTSTR lpDesID, LPCTSTR lpSrcID)
{
	VARIANT			var;

	VariantInit(&var);
	var = DHTML::GetHTMLOnClicked(lpSrcID);
	if(VT_DISPATCH != var.vt)
		return E_NOINTERFACE;
	return DHTML::SetHTMLOnClicked(lpDesID, &var);
}
//////////////////////////////////////////////////////////////////////////
 // 设置选择框Select的值
HRESULT	DHTML::SetHTMLSelectValue(LPCTSTR lpID, LPCTSTR szValue)
{
	VARIANT			vt;
	HRESULT			hr;

	VariantInit(&vt);
	vt.vt = VT_BSTR;
	vt.bstrVal = ::SysAllocString(CStringW(szValue).GetBuffer());
	hr = DHTML::SetElementProperty(lpID, DISPID_IHTMLSELECTELEMENT_VALUE, &vt);
	::SysFreeString(vt.bstrVal);
	return hr;
}
//////////////////////////////////////////////////////////////////////////
// 得到选择框Select的值
CString	DHTML::GetHTMLSelectValue(LPCTSTR lpID)
{
	CString		rtStr		= _T("");
	VARIANT		var;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLSELECTELEMENT_VALUE);
	if(VT_BSTR == var.vt)
		rtStr = var.bstrVal;
	return rtStr;
}
//////////////////////////////////////////////////////////////////////////
// 添加新项
HRESULT DHTML::HTMLSelectOptionAdd(LPCTSTR lpID, LPCTSTR szText, LPCTSTR szValue)
{
	if(FALSE == DHTML::CheckDHTMLDialogPointer(DHTML::g_pDHTMLDialog))
		return E_NOINTERFACE;
	
	IHTMLElement*			pNewElem	= NULL;
	IHTMLSelectElement*		pSelect		= NULL;
	IHTMLOptionElement*		pOption		= NULL;
	HRESULT					hr			= E_FAIL;
	CComBSTR				bstr;
	VARIANT					var;

	hr = DHTML::g_pDHTMLDialog->GetElementInterface(lpID, IID_IHTMLSelectElement, (void **)&pSelect);
	if(FAILED(hr))
		return hr;
	bstr = "OPTION";
	hr = DHTML::g_pDHTMLDialog->m_spHtmlDoc->createElement(bstr, &pNewElem);
	if(FAILED(hr))
		return hr;
	hr = pNewElem->QueryInterface(IID_IHTMLOptionElement, (void **)&pOption);
	if(FAILED(hr))
		return hr;
	bstr = szValue;
	pOption->put_value(bstr);
	bstr = szText;
	pOption->put_text(bstr);
	pOption->Release();
	VariantInit(&var);
	hr = pSelect->add(pNewElem, var);
	pNewElem->Release();
	pSelect->Release();
	return hr;
}
//////////////////////////////////////////////////////////////////////////
// 删除SELECT中的项
HRESULT DHTML::HTMLSelectOptionDelete(LPCTSTR lpID, LPCTSTR szValue)
{
	if(FALSE == DHTML::CheckDHTMLDialogPointer(DHTML::g_pDHTMLDialog))
		return E_NOINTERFACE;

	IHTMLSelectElement*		pSelect		= NULL;
	HRESULT					hr			= E_FAIL;
	LONG					nCount		= 0;
	IDispatch*				pDispatch	= NULL;
	VARIANT					var;
	IHTMLOptionElement*		pOption		= NULL;
	BSTR					bstr;

	hr = DHTML::g_pDHTMLDialog->GetElementInterface(lpID, IID_IHTMLSelectElement, (void **)&pSelect);
	if(FAILED(hr))
		return hr;
	pSelect->get_length(&nCount);
	for(int i = nCount - 1; i >= 0; i--)
	{
		VariantInit(&var);
		var.vt = VT_I4;
		var.lVal = i;
		hr = pSelect->item(var, var, &pDispatch);
		if(FAILED(hr))
			continue;
		hr = pDispatch->QueryInterface(IID_IHTMLOptionElement, (void **)&pOption);
		pDispatch->Release();
		if(FAILED(hr))
			continue;
		hr = pOption->get_value(&bstr);
		pOption->Release();
		if(CStringW(szValue).CompareNoCase(bstr) == 0)
		{
			pSelect->remove(i);
		}
	}
	pSelect->Release();
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
// 设置HTML元素的Style接口
HRESULT	DHTML::SetHTMLElementStyle(LPCTSTR lpID, LPCTSTR sname, VARIANT *vt)
{
	return DHTML::SetElementProperty(lpID, DISPID_IHTMLELEMENT_STYLE, vt);
}
//////////////////////////////////////////////////////////////////////////
// 得到HTML元素的Style接口
HRESULT	DHTML::GetHTMLElementStyle(LPCTSTR lpID, IHTMLStyle** ppStyle)
{
	VARIANT   var;

	VariantInit(&var);
	var = DHTML::GetElementProperty(lpID, DISPID_IHTMLELEMENT_STYLE);
	if(VT_DISPATCH != var.vt || NULL == var.pdispVal)
		return E_NOINTERFACE;
	IDispatch*	pDisp = var.pdispVal;
	return pDisp->QueryInterface(IID_IHTMLStyle, (void **)ppStyle);
}
//////////////////////////////////////////////////////////////////////////
// 设置HTML的可见性
HRESULT DHTML::SetHTMLElementVisible(LPCTSTR lpID, BOOL bShow)
{
	HRESULT		hr;
	IHTMLStyle*	pStyle;

	hr = DHTML::GetHTMLElementStyle(lpID, &pStyle);
	if(FAILED(hr))
		return E_NOINTERFACE;
	if(TRUE == bShow)
	{
		pStyle->put_display(L"");
	}
	else
	{
		pStyle->put_display(L"none");
	}
	pStyle->Release();
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
// 设置HTML的可见性
BOOL DHTML::GetHTMLElementVisible(LPCTSTR lpID)
{
	BOOL		bRet	= FALSE;
	BSTR		bstrDis;
	HRESULT		hr;
	IHTMLStyle*	pStyle;

	hr = DHTML::GetHTMLElementStyle(lpID, &pStyle);
	if(FAILED(hr))
		return E_NOINTERFACE;
	hr = pStyle->get_display(&bstrDis);
	if(wcslen(bstrDis) == 0)
		bRet = TRUE;
	pStyle->Release();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 模拟单击某一元素
HRESULT	DHTML::OnClickHTMLElement(LPCTSTR lpID)
{
	HRESULT				hr;
	IHTMLElement*		pElement	= NULL;

	if(FALSE == DHTML::CheckDHTMLDialogPointer(DHTML::g_pDHTMLDialog))
		return E_INVALIDARG;

	hr = DHTML::g_pDHTMLDialog->GetElement(lpID, &pElement);
	if(FAILED(hr))
		return hr;
	hr = pElement->click();
	pElement->Release();
	return hr;
}


//////////////////////////////////////////////////////////////////////////
// 检测g_pDHTMLDialog的有效性
BOOL DHTML::CheckDHTMLDialogPointer( CDHtmlDialog* const p)
{
	ASSERT(NULL != p);
	return p->IsKindOf(RUNTIME_CLASS(CDHtmlDialog));
}
