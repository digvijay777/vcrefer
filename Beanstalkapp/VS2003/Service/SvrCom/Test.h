// Test.h : CTest µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "SvrCom.h"


// CTest

class ATL_NO_VTABLE CTest : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTest, &CLSID_Test>,
	public IDispatchImpl<ITest, &IID_ITest, &LIBID_SvrComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTest()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TEST)


BEGIN_COM_MAP(CTest)
	COM_INTERFACE_ENTRY(ITest)
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

	STDMETHOD(GetMsg)(BSTR* lpVal);
};

OBJECT_ENTRY_AUTO(__uuidof(Test), CTest)
