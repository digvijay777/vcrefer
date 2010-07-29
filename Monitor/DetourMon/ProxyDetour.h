#pragma once
#include <Wininet.h>

class CProxyDetour
{
public:
	CProxyDetour(void);
	~CProxyDetour(void);

private:
	CString		m_strIni;
	BOOL		m_bHooked;
public:
	BOOL		Attach();
	BOOL		Detach();

public:
	BOOL		ConnectWidthProxy(SOCKET sock, SOCKADDR_IN& realAddr, int& nRet);
	int			UseSock4(SOCKET s, SOCKADDR_IN& realAddr);
	int			UseSock5(SOCKET s, SOCKADDR_IN& realAddr, LPCSTR lpUser, LPCSTR lpPasswd);

protected:
	BOOL		SendSocks5Query(SOCKET s, LPCSTR lpUser = NULL, LPCSTR lpPasswd = NULL);
	BOOL		LogonSocks5(SOCKET s, LPCSTR lpUser = NULL, LPCSTR lpPasswd = NULL);

public:
	static int __stdcall			dct_connect(SOCKET s, const struct sockaddr* name, int namelen);
	static HINTERNET __stdcall		dct_InternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType,LPCSTR lpszProxyName
		, LPCSTR lpszProxyBypass, DWORD dwFlags);
	static HINTERNET __stdcall		dct_InternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType,LPCWSTR lpszProxyName
		, LPCWSTR lpszProxyBypass, DWORD dwFlags);

};

extern CProxyDetour		g_proxyDetour;
