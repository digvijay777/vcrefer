// ThreadEvent.h : Declaration of the CThreadEvent

#pragma once
#include "resource.h"       // main symbols

#include "WebTest.h"
#include "_IThreadEventEvents_CP.H"

#include <atlctl.h>

// CThreadEvent

class ATL_NO_VTABLE CThreadEvent : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CThreadEvent, &CLSID_ThreadEvent>,
	public IDispatchImpl<IThreadEvent, &IID_IThreadEvent, &LIBID_WebTestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IConnectionPointContainerImpl<CThreadEvent>,
	public CProxy_IThreadEventEvents<CThreadEvent>,
	public IObjectSafetyImpl<CThreadEvent, 0>,
	public IObjectWithSiteImpl<CThreadEvent>, 
	public IProvideClassInfo2Impl<&CLSID_ThreadEvent, NULL/*&DIID__IThreadEventEvents*/, &LIBID_WebTestLib>
// 	public IDispatchImpl<_IThreadEventEvents, &__uuidof(_IThreadEventEvents), &LIBID_WebTestLib, /* wMajor = */ 1, /* wMinor = */ 0>
{
public:
	CThreadEvent()
	{
		m_dwThreadID = 0;
		m_dwGlobalTable = 0;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_THREADEVENT)


	BEGIN_COM_MAP(CThreadEvent)
		COM_INTERFACE_ENTRY(IThreadEvent)
		COM_INTERFACE_ENTRY2(IDispatch, IThreadEvent)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IObjectSafety)
		COM_INTERFACE_ENTRY(IObjectWithSite)
// 		COM_INTERFACE_ENTRY(_IThreadEventEvents)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease() 
	{
	}

	STDMETHOD ( Invoke )( DISPID dispidMember, REFIID riid, LCID lcid
		, WORD wFlags, DISPPARAMS* pDispparams, VARIANT* pVarResult
		, EXCEPINFO* pExcepinfo, UINT* puArgErr ); 
public:
	void		OnWork();
	void		OnWork(IThreadEvent* pEvents);
	DWORD		m_dwThreadID;
	DWORD		m_dwGlobalTable;

	HRESULT			GetHtmlDocument(IHTMLDocument2** pDoc);
	HRESULT			RegisterHTMLEvent(IHTMLDocument2* pDoc, DWORD* pdwCookie);
	HRESULT			UnRegisterHTMLEvent(IHTMLDocument2* pDoc, DWORD dwCookie);
	HRESULT			GetHtmlConnectionPoint(IHTMLDocument2* pDoc, IConnectionPoint** ppPoint, BOOL bBrowser = FALSE);

	enum ThreadMsg{TM_EXIT = WM_USER+2, TM_SETPROGRESS};
	static DWORD CALLBACK ThreadProc(LPVOID lpParameter);
public:

	STDMETHOD(Start)(void);
	STDMETHOD(Stop)(void);
	BEGIN_CONNECTION_POINT_MAP(CThreadEvent)
		CONNECTION_POINT_ENTRY(__uuidof(_IThreadEventEvents))
	END_CONNECTION_POINT_MAP()

	// _IThreadEventEvents Methods
public:
	STDMETHOD(OnProgress)( ULONG  nPos)
	{
		// Add your function implementation here.
		Fire_OnProgress(nPos);
		return E_NOTIMPL;
	}
	STDMETHOD(SetPos)(ULONG nPos);
};

OBJECT_ENTRY_AUTO(__uuidof(ThreadEvent), CThreadEvent)
