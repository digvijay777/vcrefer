// CallBack.h : Declaration of the CCallBack

#pragma once
#include "resource.h"       // main symbols

#include "WebTest.h"
#include <comutil.h>

// CCallBack

class ATL_NO_VTABLE CCallBack : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCallBack, &CLSID_CallBack>,
	public IDispatchImpl<ICallBack, &IID_ICallBack, &LIBID_WebTestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCallBack()
	{
		m_hThread = NULL;
		m_dwGlobalTable = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CALLBACK)


BEGIN_COM_MAP(CCallBack)
	COM_INTERFACE_ENTRY(ICallBack)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		if(NULL != m_hThread)
		{
			if(WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 0))
				TerminateThread(m_hThread, 0);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	_variant_t			m_vtCallBack;
	HANDLE				m_hThread;
	DWORD				m_dwGlobalTable;

	static DWORD CALLBACK WorkThread(LPVOID lpParameter);

	void	OnWork(IDispatch*	pDispatch);
public:

	STDMETHOD(get_CallBack)(VARIANT* pVal);
	STDMETHOD(put_CallBack)(VARIANT newVal);
	STDMETHOD(DoWork)(void);
	STDMETHOD(SetCallBack)(VARIANT var);
};

OBJECT_ENTRY_AUTO(__uuidof(CallBack), CCallBack)
