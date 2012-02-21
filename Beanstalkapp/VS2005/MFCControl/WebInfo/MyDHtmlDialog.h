#pragma once

class CMyDHtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CMyDHtmlDialog)

public:
	CMyDHtmlDialog();
	CMyDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID = 0, CWnd *pParentWnd = NULL); 
	virtual ~CMyDHtmlDialog();

public:
	// CDHtmlDialog 方法
	virtual BOOL CanAccessExternal(); // 允许访问外部对象
	virtual BOOL IsExternalDispatchSafe(); // 安全访问
 	virtual BOOL CreateControlSite(COleControlContainer* pContainer, COleControlSite** ppSite, UINT nID, REFCLSID clsid);
	
	// IDocHostUIHandler 接口
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo);
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	// IDocHostShowUI 接口
	STDMETHOD(ShowMessage)(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
		, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult);
	STDMETHOD(ShowHelp)(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
		, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit);

public:
	DECLARE_EVENTSINK_MAP()

	virtual void	OnNavigateError(LPDISPATCH pDisp, VARIANT* pvURL, VARIANT* pvFrame, VARIANT* pvStatusCode, VARIANT_BOOL* pvbCancel);
};

class CMyBrowserControlSite : public CBrowserControlSite
	, public IDocHostShowUI
{
public:
	CMyBrowserControlSite(COleControlContainer* pCtrlCont, CDHtmlDialog *pHandler);
	virtual ~CMyBrowserControlSite();

public:
	// CBrowserControlSite
	LPUNKNOWN GetInterfaceHook(const void *iid);

	// IUnknown 接口
	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IDocHostShowUI 接口
	STDMETHOD(ShowMessage)(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
		, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult);
	STDMETHOD(ShowHelp)(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
		, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit);

};