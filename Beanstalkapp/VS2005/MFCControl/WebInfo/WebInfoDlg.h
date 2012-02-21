// WebInfoDlg.h : header file
//

#pragma once


// CWebInfoDlg dialog
class CWebInfoDlg : public CDHtmlDialog 
// 	, public IDocHostShowUI
// 	, public IDocHostUIHandler
{
// Construction
public:
	CWebInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WEBINFO_DIALOG, IDH = IDR_HTML_WEBINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnBodyLButtonDown(IHTMLElement *pElement);
	HRESULT OnBodyLButtonUp(IHTMLElement *pElement);
	HRESULT OnBodyLButtonMove(IHTMLElement *pElement);
	HRESULT OnBodyContextMenu(IHTMLElement *pElement);

	HRESULT OnButtonMouseLeave(IHTMLElement *pElement);
	HRESULT OnButtonMouseHover(IHTMLElement *pElement);
	HRESULT OnButtonMouseDown(IHTMLElement *pElement);
	HRESULT OnButtonMouseUp(IHTMLElement *pElement);
	HRESULT OnNull(IHTMLElement *pElement);

	BOOL CanAccessExternal(){ return TRUE; }; //允许访问外部对象
	BOOL IsExternalDispatchSafe(){ return true; }//安全访问

public:
	// IUnknown
// 	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
// 	{
// 		if(riid == IID_IDocHostShowUI)
// 		{
// 			*ppvObject = (IDocHostShowUI *)this;
// 			return S_OK;
// 		}
// 		else if(riid == IID_IDocHostUIHandler)
// 		{
// 			*ppvObject = (IDocHostUIHandler *)this;
// 			return S_OK;
// 		}
// 		return E_NOINTERFACE;
// 	}
// 	STDMETHOD_(ULONG, AddRef)(){ return 1; }
// 	STDMETHOD_(ULONG, Release)(){ return 1; }
public:
	// IDocHostUIHandler
// 	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
// 	{
// 		return CDHtmlDialog::ShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
// 	}
// 
// 	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo)
// 	{
// 		return CDHtmlDialog::GetHostInfo(pInfo);
// 	}
// 
// 	STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget
// 		, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc)
// 	{
// 		return CDHtmlDialog::ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
// 	}
// 
// 	STDMETHOD(HideUI)( void )
// 	{
// 		return CDHtmlDialog::HideUI();
// 	}
// 
// 	STDMETHOD(UpdateUI)( void )
// 	{
// 		return CDHtmlDialog::UpdateUI();
// 	}
// 
// 	STDMETHOD(EnableModeless)(BOOL fEnable)
// 	{
// 		return CDHtmlDialog::EnableModeless(fEnable);
// 	}
// 
// 	STDMETHOD(OnDocWindowActivate)(BOOL fActivate)
// 	{
// 		return CDHtmlDialog::OnDocWindowActivate(fActivate);
// 	}
// 
// 	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate)
// 	{
// 		return CDHtmlDialog::OnFrameWindowActivate(fActivate);
// 	}
// 
// 	STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow)
// 	{
// 		return CDHtmlDialog::ResizeBorder(prcBorder, pUIWindow, fRameWindow);
// 	}
// 
// 	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID)
// 	{
// 		return CDHtmlDialog::TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
// 	}
// 
// 	STDMETHOD(GetOptionKeyPath)(LPOLESTR *pchKey, DWORD dw)
// 	{
// 		return CDHtmlDialog::GetOptionKeyPath(pchKey, dw);
// 	}
// 
// 	STDMETHOD(GetDropTarget)(IDropTarget *pDropTarget, IDropTarget **ppDropTarget)
// 	{
// 		return CDHtmlDialog::GetDropTarget(pDropTarget, ppDropTarget);
// 	}
// 
// 	STDMETHOD(GetExternal)(IDispatch **ppDispatch)
// 	{
// 		return CDHtmlDialog::GetExternal(ppDispatch);
// 	}
// 
// 	STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
// 	{
// 		return CDHtmlDialog::TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
// 	}
// 
// 	STDMETHOD(FilterDataObject)(IDataObject *pDO, IDataObject **ppDORet)
// 	{
// 		return CDHtmlDialog::FilterDataObject(pDO, ppDORet);
// 	}
public:
	// IDocHostShowUI
// 	STDMETHOD(ShowMessage)(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
// 		, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult);
// 
// 	STDMETHOD(ShowHelp)(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
// 		, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit);
// Implementation
protected:
	HICON							m_hIcon;
	CComPtr<IHTMLElement>			m_spCapture;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_EVENTSINK_MAP()

// public:
// 	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
// 	{
// 		if(riid == IID_IDocHostShowUI)
// 		{
// 			*ppvObject = (IDocHostShowUI *)this;
// 			return S_OK;
// 		}
// 		return __super::QueryInterface(riid, ppvObject);
// 	}
public:
	virtual BOOL CreateControlSite(COleControlContainer* pContainer, 
		COleControlSite** ppSite, UINT nID, REFCLSID clsid);
public:
	void	OnNavigateError(LPDISPATCH pDisp, VARIANT* pvURL,
		VARIANT* pvFrame, VARIANT* pvStatusCode, VARIANT_BOOL* pvbCancel);
public:
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
public:
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);

};


class CMyBrowserControlSite : public CBrowserControlSite 
	, public IDocHostShowUI
{
public:
	CMyBrowserControlSite(COleControlContainer* pCtrlCont, CDHtmlDialog *pHandler)
		: CBrowserControlSite(pCtrlCont, pHandler)
	{

	}
public:
	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if(riid == IID_IDocHostShowUI)
		{
			*ppvObject = (IDocHostShowUI *)this;
			return S_OK;
		}
		return CBrowserControlSite::QueryInterface(riid, ppvObject);
	}
	STDMETHOD_(ULONG, AddRef)(){ return CBrowserControlSite::AddRef(); }
	STDMETHOD_(ULONG, Release)(){ return CBrowserControlSite::Release(); }
public:
	LPUNKNOWN GetInterfaceHook(const void *iid)
	{
		if (IsEqualIID((REFIID) (*(IID*)iid), IID_IDocHostShowUI))
			return (IDocHostShowUI *) this;

		return CBrowserControlSite::GetInterfaceHook(iid);
	}
public:
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
	{
		return S_OK;
	}
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo)
	{
		HRESULT		hres	= CBrowserControlSite::GetHostInfo(pInfo);

		pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;
		return hres;
	}
public:
	STDMETHOD(ShowMessage)(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption
		, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult)
	{
		return S_OK;
	}

	STDMETHOD(ShowHelp)(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand
		, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit)
	{
		return S_OK;
	}

};