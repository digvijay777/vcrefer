// ErrTest.h : CErrTest µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "SafeArray.h"


// CErrTest

class ATL_NO_VTABLE CErrTest : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CErrTest, &CLSID_ErrTest>,
	public IDispatchImpl<IErrTest, &IID_IErrTest, &LIBID_SafeArrayLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CErrTest()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ERRTEST)


BEGIN_COM_MAP(CErrTest)
	COM_INTERFACE_ENTRY(IErrTest)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


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

OBJECT_ENTRY_AUTO(__uuidof(ErrTest), CErrTest)
