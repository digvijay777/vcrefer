#pragma once

class CMyDHtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CMyDHtmlDialog)

public:
	CMyDHtmlDialog();
	CMyDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID = 0, CWnd *pParentWnd = NULL); 
	virtual ~CMyDHtmlDialog();

public:
	// CDHtmlDialog ����
	virtual BOOL CanAccessExternal(); // ��������ⲿ����
	virtual BOOL IsExternalDispatchSafe(); // ��ȫ����
 	virtual BOOL CreateControlSite(COleControlContainer* pContainer, COleControlSite** ppSite, UINT nID, REFCLSID clsid);
	
	// IDocHostUIHandler �ӿ�
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo);
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	// IDocHostShowUI �ӿ�
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

	// IUnknown �ӿ�
	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IDocHostShowUI �ӿ�
	STDMETHOD(ShowMessage)(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
		, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult);
	STDMETHOD(ShowHelp)(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
		, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit);

};