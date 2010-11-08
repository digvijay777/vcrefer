// ErrTest2.h : CErrTest2 µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "SafeArray.h"


// CErrTest2

class ATL_NO_VTABLE CErrTest2 : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CErrTest2, &CLSID_ErrTest2>,
	public ISupportErrorInfo,
	public IDispatchImpl<IErrTest2, &IID_IErrTest2, &LIBID_SafeArrayLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CErrTest2()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ERRTEST2)


BEGIN_COM_MAP(CErrTest2)
	COM_INTERFACE_ENTRY(IErrTest2)
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

	STDMETHOD(ErrTest)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(ErrTest2), CErrTest2)
