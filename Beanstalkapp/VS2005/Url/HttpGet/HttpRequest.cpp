#include "StdAfx.h"
#include "HttpRequest.h"
#include <assert.h>

HANDLE CHttpRequest::s_thread		= NULL;
HWND CHttpRequest::s_hwnd			= NULL;

#define UM_START_HTTPREQUEST		(WM_USER+1024)
#define UM_RECV						UM_START_HTTPREQUEST+1

//////////////////////////////////////////////////////////////////////////
void MyEvtAsyncSocket::Close()
{
	__super::Close();
}

bool MyEvtAsyncSocket::Create(int af, int st, int proto, WORD wFlags/*=0*/)
{
	return __super::Create(af, st, proto, wFlags);
}

bool MyEvtAsyncSocket::Connect(LPCTSTR szAddr, unsigned short nPort)
{
	return __super::Connect(szAddr, nPort);
}

bool MyEvtAsyncSocket::Connect(const SOCKADDR* psa, int len)
{
	return __super::Connect(psa, len);
}

bool MyEvtAsyncSocket::Write(const unsigned char *pBuffIn, DWORD *pdwSize)
{
	return __super::Write(pBuffIn, pdwSize);
}

bool MyEvtAsyncSocket::Write(WSABUF *pBuffers, int nCount, DWORD *pdwSize)
{
	return __super::Write(pBuffers, nCount, pdwSize);
}

bool MyEvtAsyncSocket::Read(const unsigned char *pBuff, DWORD *pdwSize)
{
	return __super::Read(pBuff, pdwSize);
}


//////////////////////////////////////////////////////////////////////////
CHttpRequest::CHttpRequest(void)
{
}

CHttpRequest::~CHttpRequest(void)
{
}

/*
 *	开始执行
 */
BOOL CHttpRequest::StartRequest(LPCSTR lpSend, LONG nSize)
{
	if(NULL == s_thread)
	{
		return FALSE;
	}

	return ::PostMessage(s_hwnd, UM_START_HTTPREQUEST, 0, (LPARAM)this);
}

/*
 *	开启线程
 */
void CHttpRequest::StartHttp(void)
{
	if(NULL != s_thread)
	{
		return;
	}

	s_thread = CreateThread(NULL, 0, &CHttpRequest::ThreadHttpRequest, NULL, 0, NULL);
}
/*
 *	读取HTTP请求
 */
void CHttpRequest::ReadRequest(void)
{

}
/*
 *	消息分发
 */
void CHttpRequest::HttpPeekMessage()
{
	while(true)
	{
		MSG		msg		= {0};

		if( FALSE == PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			return;
		}

// 		if(UM_START_HTTPREQUEST == msg.message)
// 		{
// 			CHttpRequest*		pThis		= (CHttpRequest *)msg.lParam;
// 
// 			if(NULL == pThis)
// 			{
// 				continue;
// 			}
// 
// 		}
// 		else if(UM_RECV == msg.message)
// 		{
// 			if(sock == msg.wParam)
// 			{
// 				return; // 有数据被接收
// 			}
// 		}

		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
}

/*
 *	接收线程
 */
DWORD CALLBACK CHttpRequest::ThreadHttpRequest(LPVOID parameter)
{
	WNDCLASS		wndcls;
	const CHAR*		pClass			= "__HttpRequest__";

	memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
	// defaults

	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = &CHttpRequest::HttpWindowProc;
	wndcls.hInstance = NULL;
	wndcls.hIcon = NULL; // or load a different icon
	wndcls.hCursor = NULL;
	wndcls.hbrBackground = (HBRUSH)NULL_BRUSH;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = pClass;

	if(NULL == RegisterClass(&wndcls))
		return FALSE;
	s_hwnd = ::CreateWindow(pClass, pClass, WS_CHILD, 0, 0, 0, 0, 
		HWND_MESSAGE, NULL, NULL, NULL);
	assert(NULL != s_hwnd);

	HttpPeekMessage();
	return 0;
}

LRESULT CALLBACK CHttpRequest::HttpWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(UM_START_HTTPREQUEST == Msg)
	{
		CHttpRequest*		pThis		= (CHttpRequest *)lParam;

		if(NULL == pThis)
		{
			return 0;
		}
		pThis->Navigate("http://www.baidu.com/");
	}
	return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}