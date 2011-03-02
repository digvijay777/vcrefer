// test1.h : Declaration of the Ctest1

#pragma once
#include "resource.h"       // main symbols

#include "DoubleCOM.h"


// Ctest1

class ATL_NO_VTABLE Ctest1 : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Ctest1, &CLSID_test1>,
	public IDispatchImpl<Itest1, &IID_Itest1, &LIBID_DoubleCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<Itest2, &__uuidof(Itest2), &LIBID_DoubleCOMLib, /* wMajor = */ 1, /* wMinor = */ 0>
{
public:
	Ctest1()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_TEST1)


	BEGIN_COM_MAP(Ctest1)
		COM_INTERFACE_ENTRY(Itest1)
		COM_INTERFACE_ENTRY2(IDispatch, Itest1)
		COM_INTERFACE_ENTRY(Itest2)
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


	// Itest2 Methods
public:
	STDMETHOD(GetStr1)(BSTR* lpVal);
	STDMETHOD(GetStr2)(BSTR* lpVal);
};

OBJECT_ENTRY_AUTO(__uuidof(test1), Ctest1)
