#include "StdAfx.h"
#include ".\httpdetours.h"
#include "../../../common/Detours/detours.h"

#include "Ws2tcpip.h"

#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Ws2_32.lib")

CHTTPDetours* CHTTPDetours::s_pThis = NULL;

CHTTPDetours::CHTTPDetours(void)
{
	s_pThis = this;
	InitDetours();
}

CHTTPDetours::~CHTTPDetours(void)
{
}

/*
 *	初始化Detours
 */
void CHTTPDetours::InitDetours()
{
	m_detours.clear();

	ADD_DETOURS(InternetOpenA);
	ADD_DETOURS(InternetOpenW);
	ADD_DETOURS(InternetOpenUrlA);
	ADD_DETOURS(InternetOpenUrlW);
	ADD_DETOURS(HttpOpenRequestA);
	ADD_DETOURS(HttpOpenRequestW);
	ADD_DETOURS(InternetConnectA);
	ADD_DETOURS(InternetConnectW);
	ADD_DETOURS(send);
	ADD_DETOURS(connect);
	ADD_DETOURS(gethostbyname);
	ADD_DETOURS(getaddrinfo);
}
/*
 *	注入
 */
BOOL CHTTPDetours::AttachDetours()
{
	std::vector<Detour_Fun>::iterator		detour;

	for(detour = m_detours.begin(); detour != m_detours.end(); detour++)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach((PVOID*)detour->RealFun, (void *)detour->DetourFun);
		DetourTransactionCommit();	
	}

	return TRUE;
}

/*
 *	注销
 */
BOOL CHTTPDetours::DetachDetours()
{
	std::vector<Detour_Fun>::iterator		detour;

	for(detour = m_detours.begin(); detour != m_detours.end(); detour++)
	{
// 		DetourTransactionBegin();
// 		DetourUpdateThread(GetCurrentThread());
		DetourDetach((PVOID*)detour->RealFun, (void *)detour->DetourFun);
		DetourTransactionCommit();	
	}
	return TRUE;
}

HINTERNET WINAPI CHTTPDetours::DetourInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags)
{
	return s_pThis->RealInternetOpenA(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}

HINTERNET WINAPI CHTTPDetours::DetourInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxyName, LPCWSTR lpszProxyBypass, DWORD dwFlags)
{
	return s_pThis->RealInternetOpenW(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}

HINTERNET WINAPI CHTTPDetours::DetourInternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
	return s_pThis->RealInternetOpenUrlA(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

HINTERNET WINAPI CHTTPDetours::DetourInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
	return s_pThis->RealInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

HINTERNET WINAPI CHTTPDetours::DetourHttpOpenRequestA(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferer, LPCSTR* lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	return s_pThis->RealHttpOpenRequestA(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lpszAcceptTypes, dwFlags, dwContext);
}

HINTERNET WINAPI CHTTPDetours::DetourHttpOpenRequestW(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferer, LPCWSTR* lpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	return s_pThis->RealHttpOpenRequestW(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lpszAcceptTypes, dwFlags, dwContext);
}

HINTERNET WINAPI CHTTPDetours::DetourInternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	CHAR	szURL[]	= "www.google.cn";
	if(stricmp(lpszServerName, "www.baidu.com") == 0)
		lpszServerName = szURL;
	
	return s_pThis->RealInternetConnectA(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
}

HINTERNET WINAPI CHTTPDetours::DetourInternetConnectW(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUsername, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	return s_pThis->RealInternetConnectW(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
}

int WINAPI CHTTPDetours::Detoursend(SOCKET s, const char* buf, int len, int flags)
{
	return s_pThis->Realsend(s, buf, len, flags);
}

int WINAPI CHTTPDetours::Detourconnect(SOCKET s, const struct sockaddr* name, int namelen)
{
	return s_pThis->Realconnect(s, name, namelen);
}

struct hostent * WINAPI CHTTPDetours::Detourgethostbyname(const char* name)
{
	CHAR	szURL[]	= "www.google.cn";
	if(stricmp(name, "www.baidu.com") == 0)
		name = szURL;

	return s_pThis->Realgethostbyname(name);
}

int WINAPI CHTTPDetours::DetourWspiapiGetAddrInfo(const char* nodename, const char* servname, const struct addrinfo* hints,struct addrinfo** res)
{
	return s_pThis->RealWspiapiGetAddrInfo(nodename, servname, hints, res);
}
