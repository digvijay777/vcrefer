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
	public CComControl<CWebPlugins>
{
public:

	CWebPlugins()
	{
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
	CHAIN_MSG_MAP(CComControl<CWebPlugins>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

private:
	VPlugObject*		m_pPlugObject;

public:
	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR FAR *FAR *rgszNames, unsigned int cNames, LCID lcid, DISPID FAR *rgDispId)
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

	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pDispParams
		, VARIANT FAR *pVarResult, EXCEPINFO FAR *pExcepInfo, unsigned int FAR *puArgErr)
	{
		if(S_OK == __super::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams
			, pVarResult, pExcepInfo, puArgErr))
			return S_OK;

		if(NULL != m_pPlugObject)
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
		}

		return E_INVALIDARG;
	}
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
};

OBJECT_ENTRY_AUTO(__uuidof(WebPlugins), CWebPlugins)
