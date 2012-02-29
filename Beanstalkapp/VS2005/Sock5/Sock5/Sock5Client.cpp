#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN

#include <Winsock2.h>
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib, "Advapi32.lib")
#else
#error "Can not support!"
#endif
#include "Sock5Client.h"
/* 兼容代理服务器的连接 */
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
	// 获取代理连接信息
	if( !GetSockInfo(&nType, szAddr, &nPort, szUser, szPwd) || (4 != nType && 5 != nType))
		return connect(s, name, namelen);
	// 构建代理地址
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(nPort);
	pHost = gethostbyname(szAddr);
	if(NULL != pHost)
		sockname.sin_addr.s_addr = ((u_long FAR *)pHost->h_addr)[0];
	else
		sockname.sin_addr.S_un.S_addr = inet_addr(szAddr);
	// 开始连接
	int		iTimeOut		= 1000 * 15;	// 超时为15秒
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
	::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,(char *)&nOldSTimeout, sizeof(nOldSTimeout));

	return bRet; 
}

/* 获取代理服务器信息 */
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

	// 打开注册表
	::RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", &hKey);
	if(NULL == hKey)
		return false;
	// 读取是否有用代理协议
	dwSize = sizeof(dwProxyEnable);
	::RegQueryValueExA(hKey, "ProxyEnable", NULL, &dwType, (LPBYTE)&dwProxyEnable, &dwSize);
	if(0 == dwProxyEnable)
		goto GetSockInfo_end;
	// 读取代理的信息
	dwSize = sizeof(szData);
	::RegQueryValueExA(hKey, "ProxyServer", NULL, &dwType, (LPBYTE)szData, &dwSize);
	if(0 == szData[0])
		goto GetSockInfo_end;
	// 解析代协议设置
	// 格式ftp=[ip]:[port];gopher=[ip]:[port];http=[ip]:[port];https=[ip]:[port];socks=[ip]:[port]
	if(NULL != strchr(szData, '='))
	{
		CHAR*			pSocks		= strstr(szData, "socks");
		CHAR*			pIp			= NULL;
		
		if(NULL == pSocks)
			pSocks = strstr(szData, "http");
		if(NULL == pSocks)
			pSocks = strstr(szData, "=");
		if(NULL == pSocks)
			goto GetSockInfo_end;// 设置不对
		while('=' != *pSocks && *pSocks)
			pSocks++;
		if('=' != *pSocks)
			goto GetSockInfo_end; // 设置格式错误
		pSocks++;
		while('\x20' == *pSocks && *pSocks)
			pSocks++; // 去除空格
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
	// 分离IP和端口号
	pPoint = strchr(szIP, ':');
	if(NULL == pPoint)
		goto GetSockInfo_end;
	*pPoint = 0;
	pPoint++;
	*nType = 4;
	strcpy(pAddr, szIP);
	*nPort = atoi(pPoint);
	bRet = true;
GetSockInfo_end:
	if(NULL != hKey)
		::RegCloseKey(hKey);
	return bRet;
}

/* 通过Sock4连接 */
int __stdcall ConnectFromSock4(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen)
{
	// 构建报文
	BYTE		szData[50];
	int			iRes;

	memset(szData, 0, sizeof(szData));
	szData[0] = 0x4;			// 版本
	szData[1] = 0x1;			// 命令 CONNECT
	memcpy( &szData[2], &( ((SOCKADDR_IN*)name)->sin_port ), 2 ); // 目标机子端口
	memcpy( &szData[4], &( ((SOCKADDR_IN*)name)->sin_addr.S_un.S_addr ), 4 );	// 目标机子IP
	szData[8] = 0; // 结束标记

	// 连接代理服务器
	iRes = connect(s, sockname, nLen);
	if(ERROR_SUCCESS != iRes)
		return iRes;
	// 发送 注意要发送NULL结束符结代理服务器
	iRes = send(s, (char *)szData, 9, 0); 
	if(SOCKET_ERROR == iRes)
		return -1;
	// 接收代理服务器的返回
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, 8, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// 验证连接
	if(0 != szData[0] || 0x5A != szData[1])
		return -1;
	// 连接成功可以操作了
	return 0;
}

/* 通过Sock5连接 */
int __stdcall ConnectFromSock5(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen, const char* pUser, const char* pPwd)
{
	BYTE		szData[1024];
	int			iRes	= 0;

	// 构建报文
	memset(szData, 0, sizeof(szData));
	szData[0] = 0x5;		// 版本
	szData[1] = 0x1;		// 命令 connect
	szData[2] = 0x0;		// 保留
	// IPv4类型 - 目前只与IPv4格式， 域名格式暂略， 未处理需要密码的情况
	szData[3] = 0x1;		// IPv4
	memcpy(&szData[4], &( ((SOCKADDR_IN*)name)->sin_addr.S_un.S_addr ), 4);
	memcpy(&szData[8], &( ((SOCKADDR_IN*)name)->sin_port ), 2);
	// 连接代理服务器
	iRes = connect(s, sockname, nLen);
	if(ERROR_SUCCESS != iRes)
		return iRes;
	// 发送报文
	iRes = send(s, (char *)szData, 10, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// 接收连接
	memset(szData, 0, sizeof(szData));
	iRes = recv(s, (char *)szData, 10, 0);
	if(SOCKET_ERROR == iRes)
		return -1;
	// 验证连接
	if(5 != szData[0] || 0 != szData[1])
		return -1;
	// 连接成功可以操作了
	return 0;
}