// STAtest.h : Declaration of the CSTAtest

#pragma once
#include "resource.h"       // main symbols

#include "SafeArray.h"


// CSTAtest

class ATL_NO_VTABLE CSTAtest : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSTAtest, &CLSID_STAtest>,
	public IDispatchImpl<ISTAtest, &IID_ISTAtest, &LIBID_SafeArrayLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSTAtest()
	{
		m_Count = ++s_Count;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STATEST)


BEGIN_COM_MAP(CSTAtest)
	COM_INTERFACE_ENTRY(ISTAtest)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		// s_Count = 0;
	}
private:
	long			m_Count;
public:
	static long		s_Count;
	STDMETHOD(GetID)(LONG* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(STAtest), CSTAtest)
