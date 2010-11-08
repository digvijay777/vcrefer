// ArrayTest.h : CArrayTest µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "SafeArray.h"


// CArrayTest

class ATL_NO_VTABLE CArrayTest : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CArrayTest, &CLSID_ArrayTest>,
	public IDispatchImpl<IArrayTest, &IID_IArrayTest, &LIBID_SafeArrayLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CArrayTest()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ARRAYTEST)


BEGIN_COM_MAP(CArrayTest)
	COM_INTERFACE_ENTRY(IArrayTest)
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

	STDMETHOD(TestBSTRArray)(VARIANT* pVal);
	STDMETHOD(GetObject)(VARIANT vScript, VARIANT* lpVal);
};

OBJECT_ENTRY_AUTO(__uuidof(ArrayTest), CArrayTest)
