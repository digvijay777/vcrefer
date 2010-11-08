// TestScript.h : Declaration of the CTestScript

#pragma once
#include "resource.h"       // main symbols


// ITestScript
[
	object,
	uuid("6950CEE6-2528-4110-B680-5F838A87215C"),
	dual,	helpstring("ITestScript Interface"),
	pointer_default(unique)
]
__interface ITestScript : IDispatch
{
	[id(1), helpstring("method GetObject")] HRESULT GetObject([out,retval] VARIANT* lpVal);
};



// CTestScript

[
	coclass,
	threading("apartment"),
	vi_progid("ATLJScript.TestScript"),
	progid("ATLJScript.TestScript.1"),
	version(1.0),
	uuid("5B51CA55-DA8C-49AD-87A5-82F1AC64F3CB"),
	helpstring("TestScript Class")
]
class ATL_NO_VTABLE CTestScript : 
	public ITestScript
{
public:
	CTestScript()
	{
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(GetObject)(VARIANT* lpVal);
};

