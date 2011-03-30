// WebPlugins.h : Declaration of the CWebPlugins
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "XWebPlugins.h"
#include "_IWebPluginsEvents_CP.h"
#include "TestObject.h"

// CWebPlugins
class ATL_NO_VTABLE CWebPlugins : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IWebPlugins, &IID_IWebPlugins, &LIBID_XWebPluginsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CWebPlugins>,
	public IOleControlImpl<CWebPlugins>,
	public IOleObjectImpl<CWebPlugins>,
	public IOleInPlaceActiveObjectImpl<CWebPlugins>,
	public IViewObjectExImpl<CWebPlugins>,
	public IOleInPlaceObjectWindowlessImpl<CWebPlugins>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CWebPlugins>,
	public CProxy_IWebPluginsEvents<CWebPlugins>, 
	public IPersistStorageImpl<CWebPlugins>,
	public ISpecifyPropertyPagesImpl<CWebPlugins>,
	public IQuickActivateImpl<CWebPlugins>,
	public IDataObjectImpl<CWebPlugins>,
	public IProvideClassInfo2Impl<&CLSID_WebPlugins, &__uuidof(_IWebPluginsEvents), &LIBID_XWebPluginsLib>,
	public CComCoClass<CWebPlugins, &CLSID_WebPlugins>,
	public CComControl<CWebPlugins>,
	public VPlugDocument
{
public:

	CWebPlugins()
	{
		m_bWindowOnly = TRUE; // 指定为窗体控件
		m_pPlugObject = NULL;
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_WEBPLUGINS)

BEGIN_COM_MAP(CWebPlugins)
	COM_INTERFACE_ENTRY(IWebPlugins)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CWebPlugins)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CWebPlugins)
	CONNECTION_POINT_ENTRY(__uuidof(_IWebPluginsEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CWebPlugins)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	CHAIN_MSG_MAP(CComControl<CWebPlugins>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

private:
	VPlugObject*		m_pPlugObject;

public:
	virtual BOOL GetCookie(BSTR* lpCookie)
	{
		HRESULT							hr			= S_FALSE;
		CComPtr<IOleContainer>			spContainer;
		CComPtr<IOleClientSite>			spClientSite;
		CComPtr<IHTMLDocument2>		spDoc;

		hr = GetClientSite(&spClientSite);
		if(spClientSite == NULL)
			return FALSE;
		hr = spClientSite->GetContainer(&spContainer);
		if(spContainer == NULL)
			return FALSE;
		hr = spContainer->QueryInterface(IID_IHTMLDocument2, (void **)&spDoc);
		if(spDoc == NULL)
			return FALSE;
		hr = spDoc->get_cookie(lpCookie);
		if(FAILED(hr))
			return FALSE;
		return TRUE;
	}
public:
	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR FAR *FAR *rgszNames, unsigned int cNames, LCID lcid, DISPID FAR *rgDispId);
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pDispParams
		, VARIANT FAR *pVarResult, EXCEPINFO FAR *pExcepInfo, unsigned int FAR *puArgErr);
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_IWebPlugins,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IWebPlugins
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 7.0 : WebPlugins");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_pPlugObject = CTestObject::CreateObject();
		return S_OK;
	}
	
	void FinalRelease() 
	{
		if(NULL != m_pPlugObject)
		{
			m_pPlugObject->Release();
			m_pPlugObject = NULL;
		}
	}
public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

OBJECT_ENTRY_AUTO(__uuidof(WebPlugins), CWebPlugins)
