#pragma once
#include <atlbase.h>
#include <atlhttp.h>


class MyEvtAsyncSocket : public ZEvtSyncSocket
{
public:
	void		Close();
	bool		Create(int af, int st, int proto, WORD wFlags=0);
	bool		Connect(LPCTSTR szAddr, unsigned short nPort);
	bool		Connect(const SOCKADDR* psa, int len);
	bool		Write(const unsigned char *pBuffIn, DWORD *pdwSize);
	bool		Write(WSABUF *pBuffers, int nCount, DWORD *pdwSize);
	bool		Read(const unsigned char *pBuff, DWORD *pdwSize);

	DWORD		m_dwLastError;
// public:
// 	virtual bool	BuffWrite(const unsigned char **pBuff, DWORD *pdwSize) = 0;
};

class CHttpRequest : public CAtlHttpClientT<MyEvtAsyncSocket>
{
public:
	CHttpRequest(void);
	~CHttpRequest(void);

public:
	BOOL	StartRequest(LPCSTR lpSend, LONG nSize);
	void	ReadRequest(void);

public:
	static void		StartHttp(void);

private:
	static HANDLE		s_thread;
	static HWND			s_hwnd;

	static void				HttpPeekMessage();
	static LRESULT CALLBACK	HttpWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static DWORD CALLBACK	ThreadHttpRequest(LPVOID parameter);
};
