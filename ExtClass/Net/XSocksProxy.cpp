#include "stdafx.h"
#include "XSocksProxy.h"

CXSocksProxy::CXSocksProxy(int nVersion /* = 5 */, int nSendTimeout /* = -1 */, int nRecvTimeout /* = -1 */)
{
	m_hSocket = INVALID_SOCKET;
	m_nVersion = 0;
	m_nSendTimeout = nSendTimeout;
	m_nRecvTimeout = nRecvTimeout;
	m_nVersion = nVersion;
}

CXSocksProxy::~CXSocksProxy()
{
	Close();
}

/*
 *	连接代理服务器
 */
BOOL CXSocksProxy::Connect(LPCSTR lpIP, int nPort, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	SOCKADDR_IN			addr;
	int					iRes;
	BOOL				bRet			= FALSE;
	int					iTimeOut;

	// 调用之前要关闭
	Close();

	//////////////////////////////////////////////////////////////////////////
	// 建立连接
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.S_un.S_addr = inet_addr(lpIP);

	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == m_hSocket)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	do 
	{
		SetLastError(0);

		iTimeOut = 1000 * m_nSendTimeout; //设置发送超时6秒
		if(-1 != m_nSendTimeout &&
			SOCKET_ERROR == ::setsockopt(m_hSocket,SOL_SOCKET, SO_SNDTIMEO,(char *)&iTimeOut, sizeof(iTimeOut))) 
		{
			break;
		}

		iTimeOut = m_nRecvTimeout * 1000;//设置接收超时6秒 
		if(-1 != m_nRecvTimeout && 
			SOCKET_ERROR == ::setsockopt(m_hSocket,SOL_SOCKET, SO_RCVTIMEO,(char *)&iTimeOut, sizeof(iTimeOut))) 
		{
			break;
		}

		iRes = connect(m_hSocket, (SOCKADDR*)&addr, sizeof(addr));

		if(0 != iRes)
			break;
		
		bRet = TRUE;
	} while (false);

	if(FALSE == bRet)
	{
		Close();
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 发起请求
	// Socks5请求
	if(5 == m_nVersion)
	{
		if(FALSE == SendSocks5Query(lpUser, lpPasswd))
		{
			Close();
			return FALSE;
		}
	}

	return TRUE;
}

/*
 *	断开连接
 */
BOOL CXSocksProxy::Close()
{
	if(INVALID_SOCKET == m_hSocket)
		return TRUE;

	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	
	return TRUE;
}

/*
 *	Sockes5 连接请求
 */
BOOL CXSocksProxy::SendSocks5Query(LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE			szData[10];
	int				iRes;

	if(INVALID_SOCKET == m_hSocket)
	{
		SetLastError(SOCKET_ERROR);
		return FALSE;
	}
	// 发起请求
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// 版本标识
	szData[1] = 0x2;		// 客户能接收的类型个数
	szData[2] = 0x0;		// 第一个类型，无认证连接
	szData[3] = 0x2;		// 用户名/密码认证模式
	iRes = send(m_hSocket, (char *)szData, 4, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	memset(szData, 0, sizeof(szData));
	iRes = recv(m_hSocket, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	if(5 != szData[0])
	{
		SetLastError(PROXY_ERROR_VERSION);
		return FALSE;
	}
	if(0 == szData[1]) // 不需要验证
	{
		return TRUE;
	}
	else if(2 == szData[1]) // 需要验证
	{
		return LogonSocks5(lpUser, lpPasswd);
	}
	else
	{
		SetLastError(PROXY_ERROR_INVALID_METHOD);
	}
	
	return FALSE;
}

/*
 *	登录Socks5服务器
 */
BOOL CXSocksProxy::LogonSocks5(LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE		szData[1024];
	int			nLen			= 0;
	int			iRes;

	if(NULL == lpUser || NULL == lpPasswd)
	{
		SetLastError(PROXY_ERROR_USER);
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

	iRes = send(m_hSocket, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	// 接收返回
	memset(szData, 0, sizeof(szData));
	iRes = recv(m_hSocket, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	if(0x1 != szData[0] || 0x0 != szData[1])
	{
		SetLastError(PROXY_ERROR_USER);
		return FALSE;
	}

	return TRUE;
}

/*
 *	连接Socks4服务器
 */
BOOL CXSocksProxy::ProxySocks4(LPCSTR lpDesIP, int nPort)
{
	if(4 != m_nVersion)
	{
		SetLastError(PROXY_ERROR_VERSION);
		return FALSE;
	}

	if(INVALID_SOCKET == m_hSocket)
	{
		SetLastError(SOCKET_ERROR);
		return FALSE;
	}

	// 构建报文
	BYTE		szData[50];
	u_short		uPort;
	ULONG		ulIP;
	int			iRes;

	memset(szData, 0, sizeof(szData));
	szData[0] = 0x4;			// 版本
	szData[1] = 0x1;			// 命令 CONNECT
	uPort = htons(nPort);		
	memcpy(&szData[2], &uPort, 2); // 目标机子端口
	ulIP = inet_addr(lpDesIP);
	memcpy(&szData[4], &ulIP, 4);	// 目标机子IP
	szData[8] = 0; // 结束标记
	
	// 发送 注意要发送NULL结束符结代理服务器
	iRes = send(m_hSocket, (char *)szData, 9, 0); 
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	memset(szData, 0, sizeof(szData));
	iRes = recv(m_hSocket, (char *)szData, 8, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	if(0 != szData[0] || 0x5A != szData[1])
	{
		SetLastError(PROXY_ERROR_ALLOW);
		return FALSE;
	}

	return TRUE;
}

BOOL CXSocksProxy::ProxySocks5(LPCSTR lpDesIP, int nPort, BOOL bByIP /* = TRUE */)
{
	if(5 != m_nVersion)
	{
		SetLastError(PROXY_ERROR_VERSION);
		return FALSE;
	}
	if(INVALID_SOCKET == m_hSocket)
		return FALSE;

	BYTE		szData[1024];
	int			nLen	= 0;
	int			iRes	= 0;
	USHORT		uPort;

	// 构建报文
	uPort = htons(nPort);
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// 版本
	szData[1] = 0x1;		// 命令 connect
	szData[2] = 0x0;		// 保留

	if(FALSE != bByIP)
	{
		ULONG		ulIP;
		// IPv4类型
		szData[3] = 0x1;		// IPv4
		ulIP = inet_addr(lpDesIP);
		memcpy(&szData[4], &ulIP, 4);
		memcpy(&szData[8], &uPort, 2);
		nLen = 10;
	}
	else
	{
		// 域名方式
		szData[3] = 0x3;		// Domain
		szData[4] = strlen(lpDesIP);
		memcpy(&szData[5], lpDesIP, strlen(lpDesIP));
		nLen = 5 + strlen(lpDesIP);
		memcpy(&szData[nLen], &uPort, 2);
		nLen += 2;
	}
	
	iRes = send(m_hSocket, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	memset(szData, 0, sizeof(szData));
	iRes = recv(m_hSocket, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	if(5 != szData[0] || 0 != szData[1])
	{
		SetLastError(PROXY_ERROR_ALLOW);
		return FALSE;
	}
	
	return TRUE;
}

