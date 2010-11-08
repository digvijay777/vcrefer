// QuickTo.h : Declaration of the CQuickTo

#pragma once
#include "resource.h"       // main symbols

#include "IShellBar.h"
#include "QuickToDlg.h"
#include <Shlobj.h>

// CQuickTo

class ATL_NO_VTABLE CQuickTo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CQuickTo, &CLSID_QuickTo>,
	public IDispatchImpl<IQuickTo, &IID_IQuickTo, &LIBID_IShellBarLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CQuickTo>,
	public IObjectWithSiteImpl<CQuickTo>,
	public IDeskBand
{
public:
	CQuickTo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_QUICKTO)


BEGIN_COM_MAP(CQuickTo)
	COM_INTERFACE_ENTRY(IQuickTo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)
END_COM_MAP()

BEGIN_PROP_MAP(CQuickTo)
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
	CQuickToDlg				m_wndBar;
public:	
	BOOL					m_bRequiresSave; // IPersistStreamInitImpl 所必须的变量

	STDMETHOD(GetBandInfo)(DWORD dwBandID,	DWORD dwViewMode, DESKBANDINFO* pdbi);
	STDMETHOD(ShowDW)(BOOL bShow);
	STDMETHOD(CloseDW)(DWORD dwReserved);
	STDMETHOD(ResizeBorderDW)(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);
	STDMETHOD(GetWindow)(HWND * phwnd);
	STDMETHOD(SetSite)(IUnknown *pUnkSite);
};

OBJECT_ENTRY_AUTO(__uuidof(QuickTo), CQuickTo)
