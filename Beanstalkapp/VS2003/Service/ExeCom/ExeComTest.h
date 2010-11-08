// ExeComTest.h : CExeComTest µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "ExeCom.h"


// CExeComTest

class ATL_NO_VTABLE CExeComTest : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExeComTest, &CLSID_ExeComTest>,
	public IDispatchImpl<IExeComTest, &IID_IExeComTest, &LIBID_ExeComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CExeComTest()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXECOMTEST)


BEGIN_COM_MAP(CExeComTest)
	COM_INTERFACE_ENTRY(IExeComTest)
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

OBJECT_ENTRY_AUTO(__uuidof(ExeComTest), CExeComTest)
