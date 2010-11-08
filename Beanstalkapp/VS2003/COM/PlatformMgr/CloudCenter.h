// CloudCenter.h : Declaration of the CCloudCenter

#pragma once
#include "resource.h"       // main symbols

#include "PlatformMgr.h"
#include "_ICloudCenterEvents_CP.h"
#include <atlctl.h>

// CCloudCenter

class ATL_NO_VTABLE CCloudCenter : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCloudCenter, &CLSID_CloudCenter>,
	public IConnectionPointContainerImpl<CCloudCenter>,
	public CProxy_ICloudCenterEvents<CCloudCenter>, 
	public IObjectSafetyImpl<CCloudCenter, 0>,
	public IObjectWithSiteImpl<CCloudCenter>,
	public IDispatchImpl<ICloudCenter, &IID_ICloudCenter, &LIBID_PlatformMgrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCloudCenter()
	{
		memset(m_szData, 0, sizeof(m_szData));
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CLOUDCENTER)


BEGIN_COM_MAP(CCloudCenter)
	COM_INTERFACE_ENTRY(ICloudCenter)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CCloudCenter)
	CONNECTION_POINT_ENTRY(__uuidof(_ICloudCenterEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

private:
	wchar_t				m_szData[1024];

	DWORD				CheckLogin();

	static DWORD CALLBACK CheckLoginThread(LPVOID lpParam);
public:

	STDMETHOD(Login)(BSTR strCookie, VARIANT_BOOL *pbSuccess);
};

OBJECT_ENTRY_AUTO(__uuidof(CloudCenter), CCloudCenter)
