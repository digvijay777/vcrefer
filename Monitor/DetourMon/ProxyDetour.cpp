#include "StdAfx.h"
#include "ProxyDetour.h"
#include "detours.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wininet.lib")

// Ҫ���ص�Windows API
typedef int (__stdcall *fun_connect)(SOCKET, const struct sockaddr*, int);
typedef HINTERNET (__stdcall *fun_InternetOpenA)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
typedef HINTERNET (__stdcall *fun_InternetOpenW)(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD);

fun_connect				real_connect = &connect;
fun_InternetOpenA		real_InternetOpenA = &InternetOpenA;
fun_InternetOpenW		real_InternetOpenW = &InternetOpenW;

CProxyDetour		g_proxyDetour;

CProxyDetour::CProxyDetour(void)
{
	GetModuleFileName(GetModuleInstance(), m_strIni.GetBufferSetLength(1024), 1024);
	m_strIni.ReleaseBuffer();
	m_strIni = m_strIni.Left(m_strIni.ReverseFind('.'));
	m_strIni += ".ini";
	m_bHooked = FALSE;
}

CProxyDetour::~CProxyDetour(void)
{
}

// �ҽ�
BOOL CProxyDetour::Attach()
{
	if(FALSE == PathFileExists(m_strIni.GetBuffer()))
		return FALSE;	// �����ļ�������

	if(FALSE != m_bHooked)
		return TRUE;

	// ����sokcet
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&real_connect, (void *)dct_connect);
// 	DetourAttach((PVOID*)&real_InternetOpenA, (void *)dct_InternetOpenA);
// 	DetourAttach((PVOID*)&real_InternetOpenW, (void *)dct_InternetOpenW);
	DetourTransactionCommit();	

	return TRUE;
}

// ȡ��Ϣ�ҽ�
BOOL CProxyDetour::Detach()
{
	if(FALSE == m_bHooked)
		return FALSE;

	DetourDetach((PVOID*)real_connect, (void *)dct_connect);
// 	DetourDetach((PVOID*)real_InternetOpenA, (void *)dct_InternetOpenA);
// 	DetourAttach((PVOID*)real_InternetOpenW, (void *)dct_InternetOpenW);
	DetourTransactionCommit();	

	return TRUE;
}

/*
 *	�ҽӺ��connect
 */
int __stdcall CProxyDetour::dct_connect(SOCKET s, const struct sockaddr* name, int namelen)
{
	if(NULL != name && sizeof(SOCKADDR_IN) == namelen)
	{
		SOCKADDR_IN&		addr		= *(SOCKADDR_IN*)name;
		
		if(AF_INET == addr.sin_family)
		{
			int		nRet = 0;

			if(FALSE != g_proxyDetour.ConnectWidthProxy(s, addr, nRet))
				return nRet;
		}		
	}
	return real_connect(s, name, namelen);
}
/*
 *	�ҽӺ��InternetOpenA
 */
HINTERNET __stdcall CProxyDetour::dct_InternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType,LPCSTR lpszProxyName 
											  , LPCSTR lpszProxyBypass, DWORD dwFlags)
{
	return real_InternetOpenA(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}
HINTERNET __stdcall CProxyDetour::dct_InternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType,LPCWSTR lpszProxyName 
													, LPCWSTR lpszProxyBypass, DWORD dwFlags)
{
	return real_InternetOpenW(lpszAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}
/*
 *	��������
 */
BOOL CProxyDetour::ConnectWidthProxy(SOCKET s, SOCKADDR_IN& realAddr, int& nRet)
{
	DWORD		dwType		= 0;
	int			nLen		= sizeof(dwType);

	// �ж��׽�������
	getsockopt(s, SOL_SOCKET, SO_TYPE, (char*)&dwType, &nLen);
	if(SOCK_STREAM != dwType)
		return FALSE;

	// ����I/Oģ��
	DWORD		dwOldFlag		= 0;
	DWORD		dwFlag			= 1;
	SOCKADDR_IN	addr;
	CHAR		szIP[100]		= {0};
	CHAR		szUser[100]		= {0};
	CHAR		szPasswd[100]	= {0};
	int			nPort			= 0;
	int			nType			= 0;
	int			nTimeOut		= 3;
	fd_set		fdWrite;
	timeval		timeout;
	int			iRet			= 0;

	ioctlsocket(s, FIONREAD, &dwOldFlag);
	ioctlsocket(s, FIONBIO, &dwFlag);

	// ��ȡ����
	GetPrivateProfileString("Proxy", "Server", "", szIP, sizeof(szIP), m_strIni.GetBuffer());
	nPort = GetPrivateProfileInt("Proxy", "Port", 4, m_strIni.GetBuffer());
	nType = GetPrivateProfileInt("Proxy", "Type", 4, m_strIni.GetBuffer());
	nTimeOut = GetPrivateProfileInt("Proxy", "TimeOut", 3, m_strIni.GetBuffer());
	GetPrivateProfileString("Proxy", "User", "", szUser, sizeof(szUser), m_strIni.GetBuffer());
	GetPrivateProfileString("Proxy", "Passwd", "", szPasswd, sizeof(szUser), m_strIni.GetBuffer());

	addr.sin_addr.S_un.S_addr = inet_addr(szIP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);

	// ���Ӵ��������
	real_connect(s, (SOCKADDR*)&addr, sizeof(addr));
	FD_ZERO(&fdWrite);
	FD_SET(s, &fdWrite);
	timeout.tv_sec = nTimeOut;
	timeout.tv_usec = 0;

	iRet = select(0, NULL, &fdWrite, NULL, &timeout);
	if(iRet <= 0 || !FD_ISSET(s, &fdWrite))
	{
		// ���Ӵ��������ʧ��
		ioctlsocket(s, FIONBIO, &dwOldFlag);
		return FALSE;
	}

	// ����ͬ��
	dwFlag = 0;
	ioctlsocket(s, FIONBIO, &dwFlag);

	DWORD		dwOldRecvTmout		= 0;
	DWORD		dwRecvTmout			= 15 * 1000;	// 15��
	
	nLen = sizeof(dwOldRecvTmout);
	getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&dwOldRecvTmout, &nLen);
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&dwRecvTmout, sizeof(dwRecvTmout));
	// ���ʹ�������
	switch(nType)
	{
	case 4:	// SOCK4����
		nRet = UseSock4(s, realAddr);
		break;
	case 5: // SOCK5����
		nRet = UseSock5(s, realAddr, szUser, szPasswd);
		break;
	default:
		nRet = -1;
		break;
	}
	// ��ԭԭ����ģʽ
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&dwOldRecvTmout, sizeof(dwOldRecvTmout));
	ioctlsocket(s, FIONBIO, &dwOldFlag);

	return (0 == nRet);
}

/*
 *	ʹ��Socket4����
 */
int CProxyDetour::UseSock4(SOCKET s, SOCKADDR_IN& realAddr)
{
	BYTE		szData[50];
	int			iRes;

	// ����SOCK4����
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x4;			// �汾
	szData[1] = 0x1;			// ���� CONNECT
	memcpy(&szData[2], &realAddr.sin_port, 2); // Ŀ����Ӷ˿�
	memcpy(&szData[4], &realAddr.sin_addr.S_un.S_addr, 4);	// Ŀ�����IP
	szData[8] = 0; // �������

	// ���� ע��Ҫ����NULL����������������
	iRes = send(s, (char *)szData, 9, 0); 
	
	// �ȴ���Ӧ
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, 8, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	if(0 != szData[0] || 0x5A != szData[1])
		return -1;

	// �ɹ�����
	return 0;
}

/*
 *	ʹ��Socket5����
 */
int CProxyDetour::UseSock5(SOCKET s, SOCKADDR_IN& realAddr, LPCSTR lpUser, LPCSTR lpPasswd)
{
	if(FALSE == SendSocks5Query(s, lpUser, lpPasswd))
		return -1;	// ��¼SOCKET5����ʧ��

	// ����SOCKET5����
	BYTE		szData[1024];
	int			nLen	= 0;
	int			iRes	= 0;

	// ��������
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// �汾
	szData[1] = 0x1;		// ���� connect
	szData[2] = 0x0;		// ����

	// ��ַģʽ IPv4����
	szData[3] = 0x1;		// IPv4
	memcpy(&szData[4], &realAddr.sin_addr.S_un.S_addr, 4);
	memcpy(&szData[8], &realAddr.sin_port, 2);
	nLen = 10;
/*
		// ������ʽ
		szData[3] = 0x3;		// Domain
		szData[4] = strlen(lpDesIP);
		memcpy(&szData[5], lpDesIP, strlen(lpDesIP));
		nLen = 5 + strlen(lpDesIP);
		memcpy(&szData[nLen], &uPort, 2);
		nLen += 2;
*/

	iRes = send(s, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
		return -1;

	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
		return -1;

	if(5 != szData[0] || 0 != szData[1])
		return -1;

	// ���ӳɹ�
	return 0;
}

/*
*	Sockes5 ��������
*/
BOOL CProxyDetour::SendSocks5Query(SOCKET s, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE			szData[10];
	int				iRes;

	// ��������
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// �汾��ʶ
	szData[1] = 0x2;		// �ͻ��ܽ��յ����͸���
	szData[2] = 0x0;		// ��һ�����ͣ�����֤����
	szData[3] = 0x2;		// �û���/������֤ģʽ
	iRes = send(s, (char *)szData, 4, 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;

	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;

	if(5 != szData[0])
		return FALSE;

	if(0x0 == szData[1]) // ����Ҫ��֤
		return TRUE;
	else if(0x2 == szData[1]) // ��Ҫ��֤
		return LogonSocks5(s, lpUser, lpPasswd);

	return FALSE;
}

/*
*	��¼Socks5������
*/
BOOL CProxyDetour::LogonSocks5(SOCKET s, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE		szData[1024];
	int			nLen			= 0;
	int			iRes;

	if(NULL == lpUser || NULL == lpPasswd)
		return FALSE;

	// ������
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x1;					// ��ʶ
	szData[1] = strlen(lpUser);		// �û�������
	strncpy((char *)&szData[2], lpUser, strlen(lpUser));
	nLen = 2 + strlen(lpUser);
	szData[nLen] = strlen(lpPasswd);	// ���볤��
	strncpy((char *)&szData[nLen+1], lpPasswd, strlen(lpPasswd));
	nLen += 1 + strlen(lpPasswd);

	iRes = send(s, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;

	// ���շ���
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;
	
	// ��֤��¼
	if(0x1 != szData[0] || 0x0 != szData[1])
		return FALSE;

	return TRUE;
}