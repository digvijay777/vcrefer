#pragma once

#include <vector>
#include <WinInet.h>
#include <Winsock2.h>
//////////////////////////////////////////////////////////////////////////
#define REAL_FUN(x)			Real##x
#define DETOUR_FUN(x)		Detour##x

struct Detour_Fun
{
	void*		RealFun;
	void*		DetourFun;

	Detour_Fun(void* fn1, void* fn2)
	{
		RealFun = fn1;
		DetourFun = fn2;
	}
};

#define ADD_DETOURS(fun)\
	REAL_FUN(fun) = fun;\
	m_detours.push_back(Detour_Fun(&REAL_FUN(fun), DETOUR_FUN(fun)));\


//////////////////////////////////////////////////////////////////////////
typedef HINTERNET (WINAPI * FUN_InternetOpenA)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags);
typedef HINTERNET (WINAPI * FUN_InternetOpenW)(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxyName, LPCWSTR lpszProxyBypass, DWORD dwFlags);
typedef HINTERNET (WINAPI * FUN_InternetOpenUrlA)(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI * FUN_InternetOpenUrlW)(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI * FUN_HttpOpenRequestA)(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferer, LPCSTR* lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI * FUN_HttpOpenRequestW)(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferer, LPCWSTR* lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI * FUN_InternetConnectA)(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI * FUN_InternetConnectW)(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUsername, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
typedef int (WINAPI * FUN_send)(SOCKET s, const char* buf, int len, int flags);
typedef int (WINAPI * FUN_connect)(SOCKET s, const struct sockaddr* name, int namelen);
typedef struct hostent* (WINAPI * FUN_gethostbyname)(const char* name);
typedef int (WINAPI * FUN_getaddrinfo)(const char* nodename, const char* servname, const struct addrinfo* hints,struct addrinfo** res);



class CHTTPDetours
{
public:
	CHTTPDetours(void);
	~CHTTPDetours(void);
private:
	FUN_InternetOpenA		RealInternetOpenA;
	FUN_InternetOpenW		RealInternetOpenW;
	FUN_InternetOpenUrlA	RealInternetOpenUrlA;
	FUN_InternetOpenUrlW	RealInternetOpenUrlW;
	FUN_HttpOpenRequestA	RealHttpOpenRequestA;
	FUN_HttpOpenRequestW	RealHttpOpenRequestW;
	FUN_InternetConnectA	RealInternetConnectA;
	FUN_InternetConnectW	RealInternetConnectW;
	FUN_send				Realsend;
	FUN_connect				Realconnect;
	FUN_gethostbyname		Realgethostbyname;
	FUN_getaddrinfo			RealWspiapiGetAddrInfo;

	std::vector<Detour_Fun>		m_detours;
private:
	void		InitDetours();
public:
	BOOL		AttachDetours();
	BOOL		DetachDetours();
public:
	static		HINTERNET WINAPI DetourInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags);
	static		HINTERNET WINAPI DetourInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxyName, LPCWSTR lpszProxyBypass, DWORD dwFlags);
	static		HINTERNET WINAPI DetourInternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
	static		HINTERNET WINAPI DetourInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
	static		HINTERNET WINAPI DetourHttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferer, LPCSTR* lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
	static		HINTERNET WINAPI DetourHttpOpenRequestW(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferer, LPCWSTR* lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
	static		HINTERNET WINAPI DetourInternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
	static		HINTERNET WINAPI DetourInternetConnectW(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUsername, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
	static		int	WINAPI Detoursend(SOCKET s, const char* buf, int len, int flags);
	static		int WINAPI Detourconnect(SOCKET s, const struct sockaddr* name, int namelen);
	static		struct hostent * WINAPI Detourgethostbyname(const char* name);
	static		int WINAPI DetourWspiapiGetAddrInfo(const char* nodename, const char* servname, const struct addrinfo* hints,struct addrinfo** res);

public:
    static CHTTPDetours*		s_pThis;
};

