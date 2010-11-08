#pragma once
#include "afxsock.h"

class CMySocket :
	public CAsyncSocket
{
public:
	CMySocket(HWND hDlg);
	virtual ~CMySocket(void);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
private:
	HWND	m_hDlg;
public:
	virtual void OnConnect(int nErrorCode);
};

extern CMySocket*	g_pSocket;