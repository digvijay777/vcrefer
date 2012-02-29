#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN

#include <Winsock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Advapi32.lib")
#else
#error "Can not support!"
#endif
#include "Sock5Client.h"
/* ���ݴ�������������� */
int __stdcall ConnectEx(SOCKET s, const struct sockaddr* name, int namelen)
{
	int						nType				= 0;
	char					szAddr[128]			= {0};
	int						nPort				= 0;
	char					szUser[128]			= {0};
	char					szPwd[128]			= {0};
	SOCKADDR_IN				sockname			= {0};
	int						nLen				= sizeof(sockname);
	struct hostent*			pHost				= NULL;
	bool					bRet				= -1;

	if(NULL == name)
		return -1;
	// ��ȡ����������Ϣ
	if( !GetSockInfo(&nType, szAddr, &nPort, szUser, szPwd) || (4 != nType && 5 != nType && -1 != nType))
		return connect(s, name, namelen);
	// ���������ַ
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(nPort);
	pHost = gethostbyname(szAddr);
	if(NULL != pHost)
		sockname.sin_addr.s_addr = ((u_long FAR *)pHost->h_addr)[0];
	else
		sockname.sin_addr.S_un.S_addr = inet_addr(szAddr);
	// ��ʼ����
	int		iTimeOut		= 1000 * 15;	// ��ʱΪ15��
	int		nOldSTimeout	= 0;
	int		iSize			= sizeof(nOldSTimeout);

	::getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&nOldSTimeout, &iSize);
	::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,(char *)&iTimeOut, sizeof(iTimeOut));
	if( 4 == nType )
	{
		bRet = ConnectFromSock4(s, name, namelen, (SOCKADDR *)&sockname, nLen);
	}
	else if( 5 == nType )
	{
		bRet = ConnectFromSock5(s, name, namelen, (SOCKADDR *)&sockname, nLen, szUser, szPwd);
	}
	else
	{
		bRet = ConnectFromHttp(s, name, namelen, (SOCKADDR *)&sockname, nLen);
	}
	::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,(char *)&nOldSTimeout, sizeof(nOldSTimeout));

	return bRet; 
}

/* ��ȡ�����������Ϣ */
bool __stdcall GetSockInfo(int* nType, char* pAddr, int* nPort, char* pUser, char* pPwd)
{
	HKEY		hKey					= NULL;
	DWORD		dwProxyEnable			= 0;
	CHAR		szData[1024]			= {0};
	CHAR		szIP[128]				= {0};
	DWORD		dwType;
	DWORD		dwSize					= sizeof(dwProxyEnable);
	bool		bRet					= false;
	CHAR*		pPoint					= NULL;

	// ��ע���
	::RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", &hKey);
	if(NULL == hKey)
		return false;
	// ��ȡ�Ƿ����ô���Э��
	dwSize = sizeof(dwProxyEnable);
	::RegQueryValueExA(hKey, "ProxyEnable", NULL, &dwType, (LPBYTE)&dwProxyEnable, &dwSize);
	if(0 == dwProxyEnable)
		goto GetSockInfo_end;
	// ��ȡ�������Ϣ
	dwSize = sizeof(szData);
	::RegQueryValueExA(hKey, "ProxyServer", NULL, &dwType, (LPBYTE)szData, &dwSize);
	if(0 == szData[0])
		goto GetSockInfo_end;
	// ������Э������
	*nType = -1;
	// ��ʽftp=[ip]:[port];gopher=[ip]:[port];http=[ip]:[port];https=[ip]:[port];socks=[ip]:[port]
	if(NULL != strchr(szData, '='))
	{
		CHAR*			pSocks		= strstr(szData, "socks");
		CHAR*			pIp			= NULL;
		
		if(NULL == pSocks)
			pSocks = strstr(szData, "http");
		else
			*nType = 5;
		if(NULL == pSocks)
			pSocks = strstr(szData, "=");
		if(NULL == pSocks)
			goto GetSockInfo_end;// ���ò���
		while('=' != *pSocks && *pSocks)
			pSocks++;
		if('=' != *pSocks)
			goto GetSockInfo_end; // ���ø�ʽ����
		pSocks++;
		while('\x20' == *pSocks && *pSocks)
			pSocks++; // ȥ���ո�
		pIp = pSocks;
		pSocks = strchr(pIp, ';');
		if(NULL != pSocks)
			*pSocks = '\0';
		strcpy(szIP, pIp);
	}
	else
	{
		strncpy(szIP, szData, 128);
	}
	// ����IP�Ͷ˿ں�
	pPoint = strchr(szIP, ':');
	if(NULL == pPoint)
		goto GetSockInfo_end;
	*pPoint = 0;
	pPoint++;
	strcpy(pAddr, szIP);
	*nPort = atoi(pPoint);
	bRet = true;
GetSockInfo_end:
	if(NULL != hKey)
		::RegCloseKey(hKey);
	return bRet;
}

/* ͨ��Sock4���� */
int __stdcall ConnectFromSock4(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen)
{
	// ��������
	BYTE		szData[50];
	int			iRes;

	memset(szData, 0, sizeof(szData));
	szData[0] = 0x4;			// �汾
	szData[1] = 0x1;			// ���� CONNECT
	memcpy( &szData[2], &( ((SOCKADDR_IN*)name)->sin_port ), 2 ); // Ŀ����Ӷ˿�
	memcpy( &szData[4], &( ((SOCKADDR_IN*)name)->sin_addr.S_un.S_addr ), 4 );	// Ŀ�����IP
	szData[8] = 0; // �������

	// ���Ӵ��������
	iRes = connect(s, sockname, nLen);
	if(ERROR_SUCCESS != iRes)
		return iRes;
	// ���� ע��Ҫ����NULL����������������
	iRes = send(s, (char *)szData, 9, 0); 
	if(SOCKET_ERROR == iRes)
		return -1;
	// ���մ���������ķ���
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, 8, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// ��֤����
	if(0 != szData[0] || 0x5A != szData[1])
		return -1;
	// ���ӳɹ����Բ�����
	return 0;
}

/* ͨ��Sock5���� */
int __stdcall ConnectFromSock5(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen, const char* pUser, const char* pPwd)
{
	BYTE		szData[1024];
	int			iRes	= 0;

	// ��������
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// �汾
	szData[1] = 0x1;		// ���� connect
	szData[2] = 0x0;		// ����
	// ���Ӵ��������
	iRes = connect(s, sockname, nLen);
	if(ERROR_SUCCESS != iRes)
		return iRes;
	// ���ͱ���
	iRes = send(s, (char *)szData, 3, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// ��������
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// ��֤�Ƿ�ΪSOCK5
	if(5 != szData[0] || 0 != szData[1])
		return -1;
	// ������Ҫ���ӵĵ�ַ
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// �汾
	szData[1] = 0x1;		// ���� connect
	szData[2] = 0x0;		// ����
	// IPv4����, �ݲ�������������
	szData[3] = 0x1;		// IPv4
	memcpy(&szData[4], &( ((SOCKADDR_IN*)name)->sin_addr.S_un.S_addr ), 4);
	memcpy(&szData[8], &( ((SOCKADDR_IN*)name)->sin_port ), 2);
	if(SOCKET_ERROR == send(s, (char *)szData, 10, 0))
		return -1;
	// �ڶ��ν���
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, 10/*sizeof(szData)*/, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// ��֤����
	if(5 != szData[0] || 0 != szData[1])
		return -1;
	// ���ӳɹ����Բ�����
	return 0;
}
/* �˹�HTTP�������� */
int __stdcall ConnectFromHttp(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen)
{
	CHAR		szSend[2048]		= {0};
	int			nRet				= 0;
	int			nFlag				= 0;

	// ���Ӵ��������
	if( ERROR_SUCCESS != (nRet = connect(s, sockname, nLen)) )
		return nRet;
	// ��������
	sprintf( szSend, "CONNECT %s:%d HTTP/1.0\r\nUser-agent: MyApp/1.0\r\nConnection: Keep-Alive\r\n\r\n"
		, inet_ntoa( ((SOCKADDR_IN*)name)->sin_addr )
		, (int)ntohs( ((SOCKADDR_IN*)name)->sin_port ) );
	// ��������
	send(s, szSend, (int)strlen(szSend), 0);
	// ���շ���
	memset(szSend, 0, sizeof(szSend));
	for(int i = 0; i < (int)sizeof(szSend); i++)
	{
		if( 1 != recv(s, &szSend[i], 1, 0) )
			break;
		if('\r' == szSend[i] || '\n' == szSend[i])
			nFlag++;
		else
			nFlag = 0;
		if(4 == nFlag)
			break;
	}
	// �����ص�ֵ
	char*		pCode		= NULL;

	pCode = strchr(szSend, '\x20');
	if(NULL == pCode)
		return -1;
	pCode++;
	nRet = atoi(pCode);
	if(200 != nRet)
		return nRet;
	// ���ӳɹ������Բ�����
	return 0;
}
