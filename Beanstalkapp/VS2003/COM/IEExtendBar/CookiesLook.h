// CookiesLook.h : Declaration of the CCookiesLook

#pragma once
#include "resource.h"       // main symbols

#include "IEExtendBar.h"
#include <Shlobj.h>		// IDeskBand
#include "CookiesLookDlg.h"
// CCookiesLook

class ATL_NO_VTABLE CCookiesLook : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCookiesLook, &CLSID_CookiesLook>,
	public IDispatchImpl<ICookiesLook, &IID_ICookiesLook, &LIBID_IEExtendBarLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CCookiesLook>,
	public IObjectWithSiteImpl<CCookiesLook>,
	public IDeskBand
{
public:
	CCookiesLook()
	{

	}

DECLARE_REGISTRY_RESOURCEID(IDR_COOKIESLOOK)


BEGIN_COM_MAP(CCookiesLook)
	COM_INTERFACE_ENTRY(ICookiesLook)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	// COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)
END_COM_MAP()

BEGIN_PROP_MAP(CCookiesLook)
END_PROP_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
private:
	CCookiesLookDlg			m_wndBar;
public:
	BOOL			m_bRequiresSave; // IPersistStreamInitImpl 所必须的变量

	STDMETHOD(GetBandInfo)(DWORD dwBandID,	DWORD dwViewMode, DESKBANDINFO* pdbi);
	STDMETHOD(ShowDW)(BOOL bShow);
	STDMETHOD(CloseDW)(DWORD dwReserved);
	STDMETHOD(ResizeBorderDW)(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);
	STDMETHOD(GetWindow)(HWND * phwnd);
	STDMETHOD(SetSite)(IUnknown *pUnkSite);
};

OBJECT_ENTRY_AUTO(__uuidof(CookiesLook), CCookiesLook)
