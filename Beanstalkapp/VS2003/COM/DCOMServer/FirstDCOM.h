// FirstDCOM.h : CFirstDCOM µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "DCOMServer.h"


// CFirstDCOM

class ATL_NO_VTABLE CFirstDCOM : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFirstDCOM, &CLSID_FirstDCOM>,
	public ISupportErrorInfo,
	public IDispatchImpl<IFirstDCOM, &IID_IFirstDCOM, &LIBID_DCOMServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFirstDCOM()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIRSTDCOM)


BEGIN_COM_MAP(CFirstDCOM)
	COM_INTERFACE_ENTRY(IFirstDCOM)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(GetServerName)(BSTR* lpServer);
};

OBJECT_ENTRY_AUTO(__uuidof(FirstDCOM), CFirstDCOM)
