// LANServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include <shlwapi.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

#define IOCOMPLATEPOINT_THREAD			2
#define MAX_PACK_SIZE					(1024+512)

void			StartLicense();
DWORD WINAPI	ThreadUdpListen(LPVOID lpParameter);
DWORD WINAPI	ThreadIoCmpPoint(LPVOID lpParameter);

// 完成端口结构体
typedef struct _MYWSAOVERLAPPED : WSAOVERLAPPED
{
	DWORD				dwType;		// 是TCP还是UDP
	WSABUF				wsabuf;
	SOCKET				sock;
	SOCKADDR_IN			inaddr;
	int					inaddlen;
	DWORD				dwFlag;
	char				szData[MAX_PACK_SIZE];	// 缓冲区
}MYWSAOVERLAPPED, *PMYWSAOVERLAPPED;
// 下载文件命令
#pragma pack(push,1)
struct CMDBASE
{
	char	szCmd[32];
};

struct UpdateFileCmd : public CMDBASE
{
	DWORD			dwSize;
	FILETIME		ft;
	char			szName[];
};
#pragma pack(pop)

void		NextRecv(PMYWSAOVERLAPPED pMOP);
void		ClearTcpRecv(PMYWSAOVERLAPPED pMOP);
void		DoUpdateFile(PMYWSAOVERLAPPED pMOP);
BOOL		CheckFileTime(FILETIME* ft1, FILETIME* ft2);

// 主函数
int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// 初始化
	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		fprintf(stderr, "[WSAStartup] error: %d\n", err);
		return 0;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			fprintf(stderr, "[WSAStartup] error version(%d, %d).\n", (int)2, (int)2);
			return 0; 
	}
	// 开始监听
	StartLicense();
	// 清理
	WSACleanup();
	
	system("pause");
	return 0;
}

/*
 *	开始监听
 */
void StartLicense()
{
	HANDLE			hCmpPoint		= NULL;
	int				nErr			= 0;

	hCmpPoint = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if(NULL == hCmpPoint)
	{
		printf("创建完成端口失败: %d\n", GetLastError());
		return;
	}
	// 创建SOCKET
	SOCKET			sokTcp		= socket(AF_INET, SOCK_STREAM, 0);
	SOCKET			sokUdp		= socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN		addr		= {0};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;

	// 绑定
	nErr = bind(sokTcp, (SOCKADDR *)&addr, sizeof(addr));
	if(0 != nErr)
	{
		printf("绑定TCP失败: %d\n", WSAGetLastError());
		return;
	}
	nErr = bind(sokUdp, (SOCKADDR *)&addr, sizeof(addr));
	if(0 != nErr)
	{
		printf("绑定UDP失败: %d\n", WSAGetLastError());
		return;
	}
	// 监听
	listen(sokTcp, 5);

	// 关联完成端口
	if(NULL == CreateIoCompletionPort((HANDLE)sokUdp, hCmpPoint, NULL, 0))
	{
		printf("UDP关联完成端口失败: %d\n", GetLastError());
		return;
	}
	// 创建完成端口线程
	for(int i = 0; i < IOCOMPLATEPOINT_THREAD; i++)
	{
		HANDLE		hThread		= NULL;

		hThread = CreateThread(NULL, 0, ThreadIoCmpPoint, (void *)hCmpPoint, 0, NULL);
		if(NULL == hThread)
		{
			printf("创建完成端口线程%d失败: %d\n", i, GetLastError());
			return;
		}
	}
	// 开始临听UDP
	PMYWSAOVERLAPPED		pMOP		= new MYWSAOVERLAPPED;

	memset(pMOP, 0, sizeof(MYWSAOVERLAPPED));
	pMOP->dwType = SOCK_DGRAM;
	pMOP->wsabuf.buf = pMOP->szData;
	pMOP->wsabuf.len = sizeof(pMOP->szData);
	pMOP->inaddlen = sizeof(pMOP->inaddr);
	pMOP->sock = sokUdp;
	WSARecvFrom(sokUdp, &pMOP->wsabuf, 1, NULL, &pMOP->dwFlag
		, (SOCKADDR *)&pMOP->inaddr, &pMOP->inaddlen, pMOP, NULL);
	if(WSA_IO_PENDING != WSAGetLastError())
	{
		printf("获取数据完成端口失败: %d\n", WSAGetLastError());
		return;
	}
	// 开始临听TCP
	do 
	{
		SOCKET			sokClient		= NULL;
		SOCKADDR_IN		addr			= {0};
		int				nLen			= sizeof(addr);

		sokClient = accept(sokTcp, (SOCKADDR *)&addr, &nLen);
		printf("[TCP] 用户'%s'接入\n", inet_ntoa(addr.sin_addr));
		// 关联完成端口
		if(NULL == CreateIoCompletionPort((HANDLE)sokClient, hCmpPoint, NULL, 0))
		{
			printf("TCP关联完成端口失败: %d\n", GetLastError());
			return;
		}
		// 开始完成端口接收数据
		PMYWSAOVERLAPPED		pMOP		= new MYWSAOVERLAPPED;

		memset(pMOP, 0, sizeof(MYWSAOVERLAPPED));
		pMOP->dwType = SOCK_STREAM;
		pMOP->wsabuf.buf = pMOP->szData;
		pMOP->wsabuf.len = sizeof(pMOP->szData);
		pMOP->inaddlen = sizeof(pMOP->inaddr);
		pMOP->sock = sokClient;
		WSARecv(sokClient, &pMOP->wsabuf, 1, NULL, &pMOP->dwFlag, pMOP, NULL);
		if(WSA_IO_PENDING != WSAGetLastError())
		{
			printf("[TCP] 接收用户数据失败: %d\n", WSAGetLastError());
			delete pMOP;
			shutdown(sokClient, SD_BOTH);
			closesocket(sokClient);
		}
	} while (true);
}

// 完成商品线程
DWORD WINAPI	ThreadIoCmpPoint(LPVOID lpParameter)
{
	ULONG_PTR			*PerHandleKey;
	MYWSAOVERLAPPED     *Overlap;
	DWORD				dwBytesXfered;
	BOOL				ret;

	for (; true; NextRecv(Overlap), ClearTcpRecv(Overlap))
	{
		ret = GetQueuedCompletionStatus(
			(HANDLE)lpParameter,
			&dwBytesXfered,
			(PULONG_PTR)&PerHandleKey,
			(LPOVERLAPPED *)&Overlap,
			INFINITE);
		if (ret == 0)
		{
			printf("[Thread:%d] GetQueuedCompletionStatus failed: %d\n", GetCurrentThreadId(), GetLastError());
			break;
		}
		if(SOCK_DGRAM == Overlap->dwType)
		{
			printf("[UDP]  用户'%s'发出请求。\n", inet_ntoa(Overlap->inaddr.sin_addr));
		}
		// 处理命令
		CMDBASE*		pCmd		= (CMDBASE *)Overlap->szData;

		if(stricmp(pCmd->szCmd, "UpdateFile") != 0)
		{
			printf("暂不处理的命令\n");
			continue;
		}

		// 更新文件命令
		DoUpdateFile(Overlap);
		
	}
	return 0;
}
// UDP接收下一个命令
void NextRecv(PMYWSAOVERLAPPED pMOP)
{
	if(SOCK_DGRAM != pMOP->dwType)
		return;

	SOCKET		sock		= pMOP->sock;

	memset(pMOP, 0, sizeof(MYWSAOVERLAPPED));
	pMOP->dwType = SOCK_DGRAM;
	pMOP->wsabuf.buf = pMOP->szData;
	pMOP->wsabuf.len = sizeof(pMOP->szData);
	pMOP->inaddlen = sizeof(pMOP->inaddr);
	pMOP->sock = sock;
	WSARecvFrom(sock, &pMOP->wsabuf, 1, NULL, &pMOP->dwFlag
		, (SOCKADDR *)&pMOP->inaddr, &pMOP->inaddlen, pMOP, NULL);
	if(WSA_IO_PENDING != WSAGetLastError())
	{
		printf("获取数据完成端口失败: %d\n", WSAGetLastError());
	}
}
// TCP清除接收
void ClearTcpRecv(PMYWSAOVERLAPPED pMOP)
{
	if(SOCK_STREAM != pMOP->dwType)
		return;
	fd_set		fdset;

	FD_ZERO(&fdset);
	FD_SET(pMOP->sock, &fdset);
	select(0, NULL, &fdset, &fdset, NULL);

	shutdown(pMOP->sock, SB_BOTH);
	closesocket(pMOP->sock);
	delete pMOP;
}
// TCP-处理下载文件
void DoUpdateFile(PMYWSAOVERLAPPED pMOP)
{
	UpdateFileCmd*		pUpfile					= (UpdateFileCmd *)pMOP->szData;
	CHAR				szPath[MAX_PATH*2]		= {0};
	HANDLE				hFile					= NULL;
	FILETIME			ft						= {0};
	FILETIME			ftl						= {0};
	BYTE*				pData					= NULL;
	DWORD				dwSize					= 0;
	BOOL				bAccess					= FALSE;

	GetModuleFileNameA(NULL, szPath, sizeof(szPath));
	PathCombineA(szPath, szPath, "..\\");
	PathCombineA(szPath, szPath, pUpfile->szName);
	// 打开文件
	hFile = CreateFileA(szPath, GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		printf("打开文件'%s'失败: %d\n", szPath, GetLastError());
	}
	GetFileTime(hFile, NULL, NULL, &ft);
	FileTimeToLocalFileTime(&ft, &ftl);
	dwSize = GetFileSize(hFile, NULL);
	if(pUpfile->dwSize == dwSize
		&& CheckFileTime(&ftl, &pUpfile->ft))
	{
		bAccess = TRUE;
	}
	if(bAccess && SOCK_STREAM == pMOP->dwType)
	{
		pData = (BYTE *)malloc(dwSize);
		ReadFile(hFile, pData, dwSize, &dwSize, NULL);
	}
	CloseHandle(hFile);
	// 请求升级处理
	if(SOCK_DGRAM == pMOP->dwType)
	{
		if(bAccess)
		{
			sendto(pMOP->sock, "+", 2, 0, (SOCKADDR *)&pMOP->inaddr, pMOP->inaddlen);
		}
		return;
	}
	// 开始升级
	if(NULL == pData)
	{
		printf("不是最新文件'%s'.\n", szPath);
		return;
	}
	
	for(int i = 0; i < dwSize; )
	{
		int		nSend		= 0;

		nSend = send(pMOP->sock, (char *)pData + i, dwSize - i, 0);
		if(SOCKET_ERROR == nSend)
		{
			printf("[UpdateFile] send data to '%s' failed: %d"
				, inet_ntoa(pMOP->inaddr.sin_addr), WSAGetLastError());
			break;
		}
		i += nSend;
	}
	free(pData);
}

BOOL CheckFileTime(FILETIME* ft1, FILETIME* ft2)
{
	__int64		nRet		= 0;
	nRet = *((__int64 *)ft1) - *((__int64 *)ft2);
	if(nRet < 0)
	{
		nRet = -nRet;
	}
	if(nRet > 20*10000000)
	{
		return FALSE;//文件时间不相等
	}
	return TRUE;//文件时间相等
}