#include <WinSock2.h>
#include "XNet.h"

/*
 *	代理服务器连接
 */
BOOL XNet::ConnectWidthProxy4(SOCKET sock, LPCSTR lpDesAddr, int nDesPort, LPCSTR lpProxyAddr, int nProxyPort)
{
	// 判断句柄
	if(INVALID_SOCKET == sock)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	// 建立连接
	SOCKADDR_IN			addr;
	int					iRes			= 0;
	BOOL				bRet			= FALSE;
	BOOL				bConnect		= FALSE;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nProxyPort);
	addr.sin_addr.S_un.S_addr = GetInetAddr(lpProxyAddr);

	do 
	{
		// 连接代理服务器
		SetLastError(0);
		iRes = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
		if(0 != iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		bConnect = TRUE;

		// 连接目标机子
		BYTE		szData[50];
		u_short		uPort;
		ULONG		ulIP;
		int			iRes;

		memset(szData, 0, sizeof(szData));
		szData[0] = 0x4;			// 版本
		szData[1] = 0x1;			// 命令 CONNECT
		uPort = htons(nDesPort);		
		memcpy(&szData[2], &uPort, 2); // 目标机子端口
		ulIP = GetInetAddr(lpDesAddr);
		memcpy(&szData[4], &ulIP, 4);	// 目标机子IP
		szData[8] = 0; // 结束标记

		// 发送 注意要发送NULL结束符结代理服务器
		iRes = send(sock, (char *)szData, 9, 0); 
		if(SOCKET_ERROR == iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		// 接收信息
		memset(szData, 0, sizeof(szData));
		iRes = recv(sock, (char *)szData, 8, 0);
		if(SOCKET_ERROR == iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		// 确定连接
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
 *	代理5连接
 */
BOOL XNet::ConnectWidthProxy5(SOCKET sock, LPCSTR lpDesAddr, int nDesPort, LPCSTR lpProxyAddr
							  , int nProxyPort, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	// 判断句柄
	if(INVALID_SOCKET == sock)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	// 建立连接
	SOCKADDR_IN			addr;
	int					iRes			= 0;
	BOOL				bRet			= FALSE;
	BOOL				bConnect		= FALSE;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nProxyPort);
	addr.sin_addr.S_un.S_addr = GetInetAddr(lpProxyAddr);

	do 
	{
		// 连接代理服务器
		SetLastError(0);
		iRes = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
		if(0 != iRes)
		{
			SetLastError(WSAGetLastError());
			break;
		}
		bConnect = TRUE;
		// 查询代理服务器状态
		if(FALSE == SendProxy5Query(sock, lpUser, lpPasswd))
			break;
		// 连接目标机子
		BYTE		szData[1024];
		int			nLen	= 0;
		int			iRes	= 0;
		USHORT		uPort;

		// 构建报文
		uPort = htons(nDesPort);
		memset(szData, 0, sizeof(szData));
		szData[0] = 0x5;		// 版本
		szData[1] = 0x1;		// 命令 connect
		szData[2] = 0x0;		// 保留

		ULONG		ulIP;
		// IPv4类型
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
	// 发起请求
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// 版本标识
	szData[1] = 0x2;		// 客户能接收的类型个数
	szData[2] = 0x0;		// 第一个类型，无认证连接
	szData[3] = 0x2;		// 用户名/密码认证模式
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
	if(0 == szData[1]) // 不需要验证
	{
		return TRUE;
	}
	else if(2 == szData[1]) // 需要验证
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
*	登录Socks5服务器
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
	// 构建包
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x1;					// 标识
	szData[1] = strlen(lpUser);		// 用户名长度
	strncpy((char *)&szData[2], lpUser, strlen(lpUser));
	nLen = 2 + strlen(lpUser);
	szData[nLen] = strlen(lpPasswd);	// 密码长度
	strncpy((char *)&szData[nLen+1], lpPasswd, strlen(lpPasswd));
	nLen += 1 + strlen(lpPasswd);

	iRes = send(sock, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	// 接收返回
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
 *	得到IP地址
 */
ULONG XNet::GetInetAddr(LPCSTR lpAddr)
{
	BOOL			bDomain		= FALSE;

	// 判断方式
	for(int i = 0; i < strlen(lpAddr); i++)
	{
		if(lpAddr[i] >= 0 && lpAddr[i] <= 9)
			continue;
		else if('.' == lpAddr[i])
			continue;

		bDomain = TRUE;
		break;
	}
	// 得到IP
	if(FALSE == bDomain)
		return inet_addr(lpAddr);
	// 域名方式操作
	struct hostent*		pHostent		= NULL;

	pHostent = gethostbyname(lpAddr);
	if(NULL != pHostent)
		return *((ULONG *)pHostent->h_addr_list[0]);

	return 0;
}