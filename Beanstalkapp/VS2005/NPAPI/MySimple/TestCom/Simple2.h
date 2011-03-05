// Simple2.h : Declaration of the CSimple2

#pragma once
#include "resource.h"       // main symbols

#include "TestCom.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CSimple2

class ATL_NO_VTABLE CSimple2 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSimple2, &CLSID_Simple2>,
	public IDispatchImpl<ISimple2, &IID_ISimple2, &LIBID_TestComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSimple2()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIMPLE2)


BEGIN_COM_MAP(CSimple2)
	COM_INTERFACE_ENTRY(ISimple2)
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

};

OBJECT_ENTRY_AUTO(__uuidof(Simple2), CSimple2)
