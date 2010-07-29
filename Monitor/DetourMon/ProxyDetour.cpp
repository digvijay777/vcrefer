#include "StdAfx.h"
#include "ProxyDetour.h"
#include "detours.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wininet.lib")

// 要挂载的Windows API
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

// 挂接
BOOL CProxyDetour::Attach()
{
	if(FALSE == PathFileExists(m_strIni.GetBuffer()))
		return FALSE;	// 配置文件不存在

	if(FALSE != m_bHooked)
		return TRUE;

	// 挂载sokcet
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&real_connect, (void *)dct_connect);
// 	DetourAttach((PVOID*)&real_InternetOpenA, (void *)dct_InternetOpenA);
// 	DetourAttach((PVOID*)&real_InternetOpenW, (void *)dct_InternetOpenW);
	DetourTransactionCommit();	

	return TRUE;
}

// 取消息挂接
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
 *	挂接后的connect
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
 *	挂接后的InternetOpenA
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
 *	代理连接
 */
BOOL CProxyDetour::ConnectWidthProxy(SOCKET s, SOCKADDR_IN& realAddr, int& nRet)
{
	DWORD		dwType		= 0;
	int			nLen		= sizeof(dwType);

	// 判断套接字类型
	getsockopt(s, SOL_SOCKET, SO_TYPE, (char*)&dwType, &nLen);
	if(SOCK_STREAM != dwType)
		return FALSE;

	// 设置I/O模型
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

	// 获取配置
	GetPrivateProfileString("Proxy", "Server", "", szIP, sizeof(szIP), m_strIni.GetBuffer());
	nPort = GetPrivateProfileInt("Proxy", "Port", 4, m_strIni.GetBuffer());
	nType = GetPrivateProfileInt("Proxy", "Type", 4, m_strIni.GetBuffer());
	nTimeOut = GetPrivateProfileInt("Proxy", "TimeOut", 3, m_strIni.GetBuffer());
	GetPrivateProfileString("Proxy", "User", "", szUser, sizeof(szUser), m_strIni.GetBuffer());
	GetPrivateProfileString("Proxy", "Passwd", "", szPasswd, sizeof(szUser), m_strIni.GetBuffer());

	addr.sin_addr.S_un.S_addr = inet_addr(szIP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);

	// 连接代理服务器
	real_connect(s, (SOCKADDR*)&addr, sizeof(addr));
	FD_ZERO(&fdWrite);
	FD_SET(s, &fdWrite);
	timeout.tv_sec = nTimeOut;
	timeout.tv_usec = 0;

	iRet = select(0, NULL, &fdWrite, NULL, &timeout);
	if(iRet <= 0 || !FD_ISSET(s, &fdWrite))
	{
		// 连接代理服务器失败
		ioctlsocket(s, FIONBIO, &dwOldFlag);
		return FALSE;
	}

	// 设置同步
	dwFlag = 0;
	ioctlsocket(s, FIONBIO, &dwFlag);

	DWORD		dwOldRecvTmout		= 0;
	DWORD		dwRecvTmout			= 15 * 1000;	// 15秒
	
	nLen = sizeof(dwOldRecvTmout);
	getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&dwOldRecvTmout, &nLen);
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&dwRecvTmout, sizeof(dwRecvTmout));
	// 发送代理请求
	switch(nType)
	{
	case 4:	// SOCK4代理
		nRet = UseSock4(s, realAddr);
		break;
	case 5: // SOCK5代理
		nRet = UseSock5(s, realAddr, szUser, szPasswd);
		break;
	default:
		nRet = -1;
		break;
	}
	// 还原原来的模式
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&dwOldRecvTmout, sizeof(dwOldRecvTmout));
	ioctlsocket(s, FIONBIO, &dwOldFlag);

	return (0 == nRet);
}

/*
 *	使用Socket4代理
 */
int CProxyDetour::UseSock4(SOCKET s, SOCKADDR_IN& realAddr)
{
	BYTE		szData[50];
	int			iRes;

	// 构建SOCK4报文
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x4;			// 版本
	szData[1] = 0x1;			// 命令 CONNECT
	memcpy(&szData[2], &realAddr.sin_port, 2); // 目标机子端口
	memcpy(&szData[4], &realAddr.sin_addr.S_un.S_addr, 4);	// 目标机子IP
	szData[8] = 0; // 结束标记

	// 发送 注意要发送NULL结束符结代理服务器
	iRes = send(s, (char *)szData, 9, 0); 
	
	// 等待回应
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, 8, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	if(0 != szData[0] || 0x5A != szData[1])
		return -1;

	// 成功连接
	return 0;
}

/*
 *	使用Socket5代理
 */
int CProxyDetour::UseSock5(SOCKET s, SOCKADDR_IN& realAddr, LPCSTR lpUser, LPCSTR lpPasswd)
{
	if(FALSE == SendSocks5Query(s, lpUser, lpPasswd))
		return -1;	// 登录SOCKET5代理失败

	// 发送SOCKET5请求
	BYTE		szData[1024];
	int			nLen	= 0;
	int			iRes	= 0;

	// 构建报文
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// 版本
	szData[1] = 0x1;		// 命令 connect
	szData[2] = 0x0;		// 保留

	// 地址模式 IPv4类型
	szData[3] = 0x1;		// IPv4
	memcpy(&szData[4], &realAddr.sin_addr.S_un.S_addr, 4);
	memcpy(&szData[8], &realAddr.sin_port, 2);
	nLen = 10;
/*
		// 域名方式
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

	// 连接成功
	return 0;
}

/*
*	Sockes5 连接请求
*/
BOOL CProxyDetour::SendSocks5Query(SOCKET s, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE			szData[10];
	int				iRes;

	// 发起请求
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// 版本标识
	szData[1] = 0x2;		// 客户能接收的类型个数
	szData[2] = 0x0;		// 第一个类型，无认证连接
	szData[3] = 0x2;		// 用户名/密码认证模式
	iRes = send(s, (char *)szData, 4, 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;

	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;

	if(5 != szData[0])
		return FALSE;

	if(0x0 == szData[1]) // 不需要验证
		return TRUE;
	else if(0x2 == szData[1]) // 需要验证
		return LogonSocks5(s, lpUser, lpPasswd);

	return FALSE;
}

/*
*	登录Socks5服务器
*/
BOOL CProxyDetour::LogonSocks5(SOCKET s, LPCSTR lpUser /* = NULL */, LPCSTR lpPasswd /* = NULL */)
{
	BYTE		szData[1024];
	int			nLen			= 0;
	int			iRes;

	if(NULL == lpUser || NULL == lpPasswd)
		return FALSE;

	// 构建包
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x1;					// 标识
	szData[1] = strlen(lpUser);		// 用户名长度
	strncpy((char *)&szData[2], lpUser, strlen(lpUser));
	nLen = 2 + strlen(lpUser);
	szData[nLen] = strlen(lpPasswd);	// 密码长度
	strncpy((char *)&szData[nLen+1], lpPasswd, strlen(lpPasswd));
	nLen += 1 + strlen(lpPasswd);

	iRes = send(s, (char *)szData, nLen, 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;

	// 接收返回
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, sizeof(szData), 0);
	if(SOCKET_ERROR == iRes)
		return FALSE;
	
	// 验证登录
	if(0x1 != szData[0] || 0x0 != szData[1])
		return FALSE;

	return TRUE;
}