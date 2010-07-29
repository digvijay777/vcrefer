#pragma once


class CPreConnect
{
public:
	CPreConnect(void);
	~CPreConnect(void);

private:
	CString		m_strIni;
	BOOL		m_bHooked;
public:
	BOOL		Attach();
	BOOL		Detach();

public:
	BOOL		ConnectWidthProxy(SOCKET sock, SOCKADDR_IN& realAddr, int& nRet);
	int			UseSock4(SOCKET s, SOCKADDR_IN& realAddr, SOCKADDR_IN& addr, int nTimeOut = 3);
	int			UseSock5(SOCKET s, SOCKADDR_IN& realAddr, SOCKADDR_IN& addr, LPCSTR lpUser, LPCSTR lpPasswd, int nTimeOut = 3);

public:
	static int __stdcall	dct_connect(SOCKET s, const struct sockaddr* name, int namelen);
};

extern CPreConnect		g_preConnect;
