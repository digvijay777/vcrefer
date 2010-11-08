#include <WinSock2.h>
#include "XNet.h"

/*
 *	�������������
 */
BOOL XNet::ConnectWidthProxy4(SOCKET sock, LPCSTR lpDesAddr, int nDesPort, LPCSTR lpProxyAddr, int nProxyPort)
{
	// �жϾ��
	if(INVALID_SOCKET == sock)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	// ��������
	SOCKADDR_IN			addr;
	int					iRes			= 0;
	BOOL				bRet			= FALSE;
	BOOL				bConnect		= FALSE;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nProxyPort);
	addr.sin_addr.S_un.S_addr = GetInetAddr(lpProxyAddr);

	do 
	{
		// ���Ӵ��������
		SetLastError(0);
		iRes = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
		if(0 != iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		bConnect = TRUE;

		// ����Ŀ�����
		BYTE		szData[50];
		u_short		uPort;
		ULONG		ulIP;
		int			iRes;

		memset(szData, 0, sizeof(szData));
		szData[0] = 0x4;			// �汾
		szData[1] = 0x1;			// ���� CONNECT
		uPort = htons(nDesPort);		
		memcpy(&szData[2], &uPort, 2); // Ŀ����Ӷ˿�
		ulIP = GetInetAddr(lpDesAddr);
		memcpy(&szData[4], &ulIP, 4);	// Ŀ�����IP
		szData[8] = 0; // �������

		// ���� ע��Ҫ����NULL����������������
		iRes = send(sock, (char *)szData, 9, 0); 
		if(SOCKET_ERROR == iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		// ������Ϣ
		memset(szData, 0, sizeof(szData));
		iRes = recv(sock, (char *)szData, 8, 0);
		if(SOCKET_ERROR == iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		// ȷ������
		if(0 != szData[0] || 0x5A != szData[1])
		{
			SetLastError(-1);
			break;
		}
		bRet = TRUE;

	} while (false);

	if(!bRet && bConnect)
	{
		shutdown(sock, SD_BOTH);
		closesocket(sock);
	}

	return bRet;
}
/*
 *	����5����
 */
BOOL XNet::ConnectWidthProxy5(SOCKET sock, LPCSTR lpDesAddr, int nDesPort, LPCSTR lpProxyAddr
							  , int nProxyPort, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	// �жϾ��
	if(INVALID_SOCKET == sock)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	// ��������
	SOCKADDR_IN			addr;
	int					iRes			= 0;
	BOOL				bRet			= FALSE;
	BOOL				bConnect		= FALSE;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nProxyPort);
	addr.sin_addr.S_un.S_addr = GetInetAddr(lpProxyAddr);

	do 
	{
		// ���Ӵ��������
		SetLastError(0);
		iRes = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
		if(0 != iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		bConnect = TRUE;
		// ��ѯ���������״̬
		if(FALSE == SendProxy5Query(sock, lpUser, lpPasswd))
			break;
		// ����Ŀ�����
		BYTE		szData[1024];
		int			nLen	= 0;
		int			iRes	= 0;
		USHORT		uPort;

		// ��������
		uPort = htons(nDesPort);
		memset(szData, 0, sizeof(szData));
		szData[0] = 0x5;		// �汾
		szData[1] = 0x1;		// ���� connect
		szData[2] = 0x0;		// ����

		ULONG		ulIP;
		// IPv4����
		szData[3] = 0x1;		// IPv4
		ulIP = GetInetAddr(lpDesAddr);
		memcpy(&szData[4], &ulIP, 4);
		memcpy(&szData[8], &uPort, 2);
		nLen = 10;
	
		iRes = send(sock, (char *)szData, nLen, 0);
		if(SOCKET_ERROR == iRes)
		{
			SetLastError(WSAGetLastError());
			return FALSE;
		}
		memset(szData, 0, sizeof(szData));
		iRes = recv(sock, (char *)szData, nLen, 0);
		if(SOCKET_ERROR == iRes)
		{
			SetLastError(WSAGetLastError());
			return FALSE;
		}

		if(5 != szData[0] || 0 != szData[1])
		{
			SetLastError(ERROR_ACCESS_DENIED);
			return FALSE;
		}

		bRet = TRUE;

	} while (false);

	if(!bRet && bConnect)
	{
		shutdown(sock, SD_BOTH);
		closesocket(sock);
	}


	return bRet;
}

BOOL XNet::SendProxy5Query(SOCKET sock, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE			szData[10];
	int				iRes;

	if(INVALID_SOCKET == sock)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	// ��������
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// �汾��ʶ
	szData[1] = 0x2;		// �ͻ��ܽ��յ����͸���
	szData[2] = 0x0;		// ��һ�����ͣ�����֤����
	szData[3] = 0x2;		// �û���/������֤ģʽ
	iRes = send(sock, (char *)szData, 4, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	memset(szData, 0, sizeof(szData));
	iRes = recv(sock, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	if(5 != szData[0])
	{
		SetLastError(-1);
		return FALSE;
	}
	if(0 == szData[1]) // ����Ҫ��֤
	{
		return TRUE;
	}
	else if(2 == szData[1]) // ��Ҫ��֤
	{
		return LoginProxy5(sock, lpUser, lpPasswd);
	}
	else
	{
		SetLastError(-1);
	}

	return FALSE;
}

/*
*	��¼Socks5������
*/
BOOL XNet::LoginProxy5(SOCKET sock, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE		szData[1024];
	int			nLen			= 0;
	int			iRes;

	if(NULL == lpUser || NULL == lpPasswd)
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	// ������
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x1;					// ��ʶ
	szData[1] = strlen(lpUser);		// �û�������
	strncpy((char *)&szData[2], lpUser, strlen(lpUser));
	nLen = 2 + strlen(lpUser);
	szData[nLen] = strlen(lpPasswd);	// ���볤��
	strncpy((char *)&szData[nLen+1], lpPasswd, strlen(lpPasswd));
	nLen += 1 + strlen(lpPasswd);

	iRes = send(sock, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	// ���շ���
	memset(szData, 0, sizeof(szData));
	iRes = recv(sock, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	if(0x1 != szData[0] || 0x0 != szData[1])
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return FALSE;
	}

	return TRUE;
}
/*
 *	�õ�IP��ַ
 */
ULONG XNet::GetInetAddr(LPCSTR lpAddr)
{
	BOOL			bDomain		= FALSE;

	// �жϷ�ʽ
	for(int i = 0; i < strlen(lpAddr); i++)
	{
		if(lpAddr[i] >= 0 && lpAddr[i] <= 9)
			continue;
		else if('.' == lpAddr[i])
			continue;

		bDomain = TRUE;
		break;
	}
	// �õ�IP
	if(FALSE == bDomain)
		return inet_addr(lpAddr);
	// ������ʽ����
	struct hostent*		pHostent		= NULL;

	pHostent = gethostbyname(lpAddr);
	if(NULL != pHostent)
		return *((ULONG *)pHostent->h_addr_list[0]);

	return 0;
}