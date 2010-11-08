// TestDCOM.h : Declaration of the CTestDCOM

#pragma once
#include "resource.h"       // main symbols

#include "DCOMTest.h"


// CTestDCOM

class ATL_NO_VTABLE CTestDCOM : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTestDCOM, &CLSID_TestDCOM>,
	public IDispatchImpl<ITestDCOM, &IID_ITestDCOM, &LIBID_DCOMTestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTestDCOM()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TESTDCOM)


BEGIN_COM_MAP(CTestDCOM)
	COM_INTERFACE_ENTRY(ITestDCOM)
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

	STDMETHOD(MessageBox)(BSTR pMsg);
};

OBJECT_ENTRY_AUTO(__uuidof(TestDCOM), CTestDCOM)
