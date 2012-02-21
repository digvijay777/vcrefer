// MyDHtmlDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyDHtmlDialog.h"


IMPLEMENT_DYNCREATE(CMyDHtmlDialog, CDHtmlDialog)

CMyDHtmlDialog::CMyDHtmlDialog()
: CDHtmlDialog()
{

}

CMyDHtmlDialog::CMyDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID /* = 0 */, CWnd *pParentWnd /* = NULL */)
: CDHtmlDialog(nIDTemplate, nHtmlResID, pParentWnd)
{

}

CMyDHtmlDialog::~CMyDHtmlDialog()
{
}

BEGIN_EVENTSINK_MAP(CMyDHtmlDialog, CDHtmlDialog)
	ON_EVENT(CMyDHtmlDialog, AFX_IDC_BROWSER, 271 /*271 DISPID_NAVIGATEERROR */, OnNavigateError, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()
//////////////////////////////////////////////////////////////////////////
// CDHtmlDialog 虚函数
BOOL CMyDHtmlDialog::CanAccessExternal()
{
	return TRUE;
}

BOOL CMyDHtmlDialog::IsExternalDispatchSafe()
{
	return TRUE;
}
// 挂接DHtml事件
BOOL CMyDHtmlDialog::CreateControlSite(COleControlContainer* pContainer, COleControlSite** ppSite, UINT nID, REFCLSID clsid)
{
	if(ppSite == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CMyBrowserControlSite *pBrowserSite = 
		new CMyBrowserControlSite(pContainer, this);
	if (!pBrowserSite)
		return FALSE;

	*ppSite = pBrowserSite;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// IDocHostUIHandler 接口
STDMETHODIMP CMyDHtmlDialog::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	HRESULT		hres	= CDHtmlDialog::GetHostInfo(pInfo);

	pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;	// 消除选择文本
	return hres;
}
STDMETHODIMP CMyDHtmlDialog::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	return S_OK; // 不显示IE的右键菜单
}

//////////////////////////////////////////////////////////////////////////
// IDocHostShowUI 接口
STDMETHODIMP CMyDHtmlDialog::ShowMessage(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
												, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult)
{
	return S_FALSE;
}

STDMETHODIMP CMyDHtmlDialog::ShowHelp(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
											 , DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit)
{
	return S_FALSE;
}

// 导航错误时的回调函数
void CMyDHtmlDialog::OnNavigateError(LPDISPATCH pDisp, VARIANT* pvURL, VARIANT* pvFrame, VARIANT* pvStatusCode, VARIANT_BOOL* pvbCancel)
{

}
/************************************************************************/
/* CMyBrowserControlSite 类                                             */
/************************************************************************/
CMyBrowserControlSite::CMyBrowserControlSite(COleControlContainer* pCtrlCont, CDHtmlDialog *pHandler)
: CBrowserControlSite(pCtrlCont, pHandler)
{

}

CMyBrowserControlSite::~CMyBrowserControlSite()
{

}

//////////////////////////////////////////////////////////////////////////
// CBrowserControlSite
LPUNKNOWN CMyBrowserControlSite::GetInterfaceHook(const void *iid)
{
	if( IsEqualIID((REFIID) (*(IID*)iid), IID_IDocHostShowUI) )
	{
		return (IDocHostShowUI *)this;
	}

	return CBrowserControlSite::GetInterfaceHook(iid);
}

//////////////////////////////////////////////////////////////////////////
// IUnknown
STDMETHODIMP CMyBrowserControlSite::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if(riid == IID_IDocHostShowUI)
	{
		*ppvObject = (IDocHostShowUI *)this;
		AddRef();
		return S_OK;
	}
	return CBrowserControlSite::QueryInterface(riid, ppvObject);
}

STDMETHODIMP_(ULONG) CMyBrowserControlSite::AddRef()
{
	return CBrowserControlSite::AddRef();
}

STDMETHODIMP_(ULONG) CMyBrowserControlSite::Release()
{
	return CBrowserControlSite::Release();
}
//////////////////////////////////////////////////////////////////////////
// IDocHostShowUI 接口
STDMETHODIMP CMyBrowserControlSite::ShowMessage(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
										 , DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult)
{
	if(NULL != m_pHandler && m_pHandler->IsKindOf(RUNTIME_CLASS(CMyDHtmlDialog)))
		return ((CMyDHtmlDialog *)m_pHandler)->ShowMessage(hwnd, lpstrText, lpstrCaption, dwType, lpstrHelpFile, dwHelpContext, plResult);
	return S_NORMAL;
}

STDMETHODIMP CMyBrowserControlSite::ShowHelp(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
									  , DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit)
{
	if(NULL != m_pHandler && m_pHandler->IsKindOf(RUNTIME_CLASS(CMyDHtmlDialog)))
		return ((CMyDHtmlDialog *)m_pHandler)->ShowHelp(hwnd, pszHelpFile, uCommand, dwData, ptMouse, pDispatchObjectHit);
	return S_NORMAL;
}