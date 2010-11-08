// ThreadEvent.cpp : Implementation of CThreadEvent

#include "stdafx.h"
#include "ThreadEvent.h"
#include ".\threadevent.h"
#include <ExDispid.h>
#include <MsHtmdid.h>

// CThreadEvent


STDMETHODIMP CThreadEvent::Start(void)
{
	HANDLE		hThread;
	DWORD		dwRes;

	/************************************************************************/
	/* ���׼�ʹ��                                                            */
	/************************************************************************/
	CComPtr<IGlobalInterfaceTable>		spGlobal;

	spGlobal.CoCreateInstance(CLSID_StdGlobalInterfaceTable);
	((IThreadEvent *)this)->AddRef();
	//g_spGlobalTable->RegisterInterfaceInGlobal((IConnectionPoint* )this, __uuidof(IConnectionPoint), &m_dwGlobalTable);
	spGlobal->RegisterInterfaceInGlobal((IThreadEvent* )this, __uuidof(IThreadEvent), &m_dwGlobalTable);
	//////////////////////////////////////////////////////////////////////////
	hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	
	// ע����ҳ�¼�
	CComPtr<IHTMLDocument2>			spDoc;
	DWORD							dwCookie		= 0;
	HRESULT							hres;
	
	hres = GetHtmlDocument(&spDoc);
	RegisterHTMLEvent(spDoc, &dwCookie);

	m_dwThreadID = GetCurrentThreadId();
	while(true)
	{
		MSG msg; 

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			switch(msg.message)
			{
			case WM_QUIT:
			case WM_CLOSE:
			case WM_DESTROY:
// 				MessageBox(NULL, "Thread Event is Running.\n"
// 					"Yon can not close this.\n"
// 					"Please Click 'Stop' button befor.", "WebTest", MB_OK | MB_ICONWARNING);
				continue;
			case TM_EXIT:	// �˳��߳�
				TerminateThread(hThread, -1);
				continue;
			case TM_SETPROGRESS:		// ���ý���
				ATLTRACE("Set Progress %d\n", msg.wParam);
				Fire_OnProgress((ULONG)msg.wParam);
				continue;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			if(WAIT_OBJECT_0 == WaitForSingleObject(hThread, 0))
				break;
		}
		dwRes = MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT|QS_ALLPOSTMESSAGE);
		if(WAIT_OBJECT_0 == dwRes)
		{
			// �߳��˳�
			Fire_OnProgress(100);
			break;
		}
	}

	UnRegisterHTMLEvent(spDoc, dwCookie);
	CloseHandle(hThread);

	return S_OK;
}

STDMETHODIMP CThreadEvent::Stop(void)
{
	PostThreadMessage(m_dwThreadID, TM_EXIT, 0, 0);

	return S_OK;
}

DWORD CALLBACK CThreadEvent::ThreadProc(LPVOID lpParameter)
{
	CThreadEvent*	pThis		= (CThreadEvent *)lpParameter;
	
	//pThis->OnWork();

	//////////////////////////////////////////////////////////////////////////
// 	pThis->OnWork(NULL);

	::CoInitialize(NULL);

	CComPtr<IGlobalInterfaceTable>		spGlobal;
	IThreadEvent*		pEvent		= NULL;

	spGlobal.CoCreateInstance(CLSID_StdGlobalInterfaceTable);

	spGlobal->GetInterfaceFromGlobal(pThis->m_dwGlobalTable, __uuidof(IThreadEvent), (void **)&pEvent);
	if(NULL == pEvent)
	{
		ATLASSERT(NULL != pEvent);
		return 0;
	}
	g_spGlobalTable->RevokeInterfaceFromGlobal(pThis->m_dwGlobalTable);
	pThis->m_dwGlobalTable = 0;
	pThis->OnWork(pEvent);
	// pThis->OnWork(pEvent);
	pEvent->Release();
	

	return 0;
}
void CThreadEvent::OnWork(IThreadEvent* pEvents)
{
// 	for(int i = 0; i < 100; i++)
// 	{
// 		CComVariant avarParams[1];
// 		avarParams[0] = i;	
// 		avarParams[0].vt = VT_UI4;
// 		CComVariant varResult;
// 
// 		DISPPARAMS params = { avarParams, NULL, 1, 0 };
// 		pEvents->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
// 		Sleep(1000);
// 	}
	for(int i = 0; i < 100; i++)
	{
		//Fire_OnProgress(i);
		pEvents->SetPos(i);
		Sleep(1000);
	}
}
void CThreadEvent::OnWork()
{
	for(int i = 0; i < 100; i++)
	{
		PostThreadMessage(m_dwThreadID, TM_SETPROGRESS, i, 0);
		Sleep(1000);
	}
}

/*
 *	�õ���ҳ
 */
HRESULT CThreadEvent::GetHtmlDocument(IHTMLDocument2** pDoc)
{
	HRESULT							hres;
	CComPtr<IOleClientSite>			spOleSite;

	// �õ�ͣ����
	hres = GetSite(IID_IOleClientSite, (void **)&spOleSite);
	if(FAILED(hres) || spOleSite == NULL)
		return E_NOINTERFACE;

	// �õ���ҳ�ӿ�
	CComPtr<IOleContainer>			spContainer;

 	hres = spOleSite->GetContainer(&spContainer);
	if(FAILED(hres) || spContainer == NULL)
		return E_NOINTERFACE;

	hres = spContainer->QueryInterface(IID_IHTMLDocument2, (void **)pDoc);

	return hres;
}
/*
 *	�õ���ҳ���ӵ�
 */
HRESULT CThreadEvent::GetHtmlConnectionPoint(IHTMLDocument2* pDoc, IConnectionPoint** ppPoint, BOOL bBrowser /* = FALSE */)
{
	HRESULT									hres;
	CComPtr<IServiceProvider>				spISP;
	CComQIPtr<IWebBrowser2>					spWeb;
	CComPtr<IConnectionPointContainer>		spPoint;
	CComPtr<IHTMLWindow2>					spWnd;

	if(NULL == pDoc)
		return E_NOINTERFACE;

	if(FALSE != bBrowser)
	{
		// ������¼�
		// ��ȡService
		hres = pDoc->QueryInterface(IID_IServiceProvider, (void **)&spISP);
		if(FAILED(hres))
			return E_NOINTERFACE;
		// ��ȡ�����
		hres = spISP->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void **)&spWeb);
		if(FAILED(hres))
			return E_NOINTERFACE;
		// ���ӵ�
		hres = spWeb->QueryInterface(IID_IConnectionPointContainer, (void **)&spPoint);
		if(FAILED(hres))
			return E_NOINTERFACE;
		hres = spPoint->FindConnectionPoint(DIID_DWebBrowserEvents2, ppPoint);
		if(FAILED(hres) || NULL == *ppPoint)
			return E_NOINTERFACE;
	}
	else
	{
		// Docment���¼�
		hres = pDoc->get_parentWindow(&spWnd);
		if(FAILED(hres))
			return E_NOINTERFACE;
		hres = spWnd->QueryInterface(IID_IConnectionPointContainer, (void **)&spPoint);
		if(FAILED(hres))
			return E_NOINTERFACE;
		hres = spPoint->FindConnectionPoint(DIID_HTMLWindowEvents/*DIID_HTMLDocumentEvents*/, ppPoint);
		if(FAILED(hres) || NULL == *ppPoint)
			return E_NOINTERFACE;
	}

	return S_OK;
}
/*
 *	ע���˳��¼�
 */
HRESULT CThreadEvent::RegisterHTMLEvent(IHTMLDocument2* pDoc, DWORD* pdwCookie)
{
	CComPtr<IConnectionPoint>		spPoint;
	HRESULT							hres;

	hres = GetHtmlConnectionPoint(pDoc, &spPoint);
	if(FAILED(hres))
		return E_FAIL;
	
	hres = spPoint->Advise(reinterpret_cast<IDispatch*>(this), pdwCookie);

	return hres;
}

/*
 *	ע���¼�
 */
HRESULT CThreadEvent::UnRegisterHTMLEvent(IHTMLDocument2* pDoc, DWORD dwCookie)
{
	CComPtr<IConnectionPoint>		spPoint;
	HRESULT							hres;

	hres = GetHtmlConnectionPoint(pDoc, &spPoint);
	if(FAILED(hres))
		return E_FAIL;

	hres = spPoint->Unadvise(dwCookie);

	return hres;
}
/*
 *	������ҳ�¼�
 */
STDMETHODIMP CThreadEvent::Invoke(DISPID dispidMember, REFIID riid, LCID lcid
								  , WORD wFlags, DISPPARAMS* pDispparams, VARIANT* pVarResult
								  , EXCEPINFO* pExcepinfo, UINT* puArgErr)
{
	switch(dispidMember)
	{
	case DISPID_ONBEFOREUNLOAD:
		*pVarResult = CComVariant("WebTest ���ڹ����У����鲻Ҫǿ���˳���������в���֪�Ĵ�������");
		return S_OK;
	case DISPID_ONUNLOAD:
		PostThreadMessage(m_dwThreadID, TM_EXIT, 0, 0);
		return S_OK;
	case DISPID_DOCUMENTCOMPLETE:
		return S_OK;
	case DISPID_PROGRESSCHANGE:
		return S_OK;
	case DISPID_NAVIGATEERROR:
		return S_OK;
	case DISPID_BEFORENAVIGATE2:
		return S_OK;
	case DISPID_DOWNLOADBEGIN:
		return S_OK;
	case DISPID_HTMLDOCUMENTEVENTS_ONBEFOREACTIVATE:
		*pVarResult = CComVariant("Web Test is working and can not close.");
		return E_FAIL;
	}

// 	return IDispatchImpl<IThreadEvent, &IID_IThreadEvent, &LIBID_WebTestLib, 1, 0>::Invoke(dispidMember, riid, lcid, wFlags
// 		 		, pDispparams, pVarResult, pExcepinfo, puArgErr);
	return __super::Invoke(dispidMember, riid, lcid, wFlags
		, pDispparams, pVarResult, pExcepinfo, puArgErr);
}
STDMETHODIMP CThreadEvent::SetPos(ULONG nPos)
{
	// TODO: Add your implementation code here
	
	Fire_OnProgress(nPos);
	return S_OK;
}
