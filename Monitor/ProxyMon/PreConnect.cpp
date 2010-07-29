#include "StdAfx.h"
#include ".\preconnect.h"
#include "detours.h"

#pragma comment(lib, "Ws2_32.lib")

typedef int (__stdcall* fun_connect)(SOCKET, const struct sockaddr*, int);

fun_connect		real_connect = &connect;

CPreConnect		g_preConnect;

CPreConnect::CPreConnect(void)
{
	GetModuleFileName(GetModuleInstance(), m_strIni.GetBufferSetLength(1024), 1024);
	m_strIni.ReleaseBuffer();
	m_strIni = m_strIni.Left(m_strIni.ReverseFind('.'));
	m_strIni += ".ini";
	m_bHooked = FALSE;
}

CPreConnect::~CPreConnect(void)
{
}

// �ҽ�
BOOL CPreConnect::Attach()
{
	if(FALSE == PathFileExists(m_strIni.GetBuffer()))
		return FALSE;	// �����ļ�������

	if(FALSE != m_bHooked)
		return TRUE;

	// ����sokcet
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&real_connect, (void *)dct_connect);
	DetourTransactionCommit();	

	return TRUE;
}

// ȡ��Ϣ�ҽ�
BOOL CPreConnect::Detach()
{
	if(FALSE == m_bHooked)
		return FALSE;

	DetourDetach((PVOID*)real_connect, (void *)dct_connect);
	DetourTransactionCommit();	

	return TRUE;
}

/*
 *	�ҽӺ��connect
 */
int __stdcall CPreConnect::dct_connect(SOCKET s, const struct sockaddr* name, int namelen)
{
	if(NULL != name && sizeof(SOCKADDR_IN) == namelen)
	{
		SOCKADDR_IN&		addr		= *(SOCKADDR_IN*)name;
		
		if(AF_INET == addr.sin_family)
		{
			int		nRet = 0;

			if(FALSE != g_preConnect.ConnectWidthProxy(s, addr, nRet))
				return nRet;
		}		
	}
	return real_connect(s, name, namelen);
}

/*
 *	��������
 */
BOOL CPreConnect::ConnectWidthProxy(SOCKET s, SOCKADDR_IN& realAddr, int& nRet)
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
	switch(nType)
	{
	case 4:	// SOCK4����
		nRet = UseSock4(s, realAddr, addr, nTimeOut);
		break;
	case 5: // SOCK5����
		nRet = UseSock5(s, realAddr, addr, szUser, szPasswd, nTimeOut);
		break;
	default:
		nRet = -1;
		break;
	}
	// ��ԭԭ����ģʽ
	ioctlsocket(s, FIONBIO, &dwOldFlag);

	return (0 == nRet);
}

/*
 *	ʹ��Socket4����
 */
int CPreConnect::UseSock4(SOCKET s, SOCKADDR_IN& realAddr, SOCKADDR_IN& addr, int nTimeOut)
{
	return -1;
}

/*
 *	ʹ��Socket5����
 */
int CPreConnect::UseSock5(SOCKET s, SOCKADDR_IN& realAddr, SOCKADDR_IN& addr, LPCSTR lpUser
						  , LPCSTR lpPasswd, int nTimeOut /* = 3 */)
{
	return -1;
}

