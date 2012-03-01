// Sock5.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include "Sock5Client.h"
#include <process.h>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")

void InitSock();
void myexit();

int _tmain(int argc, _TCHAR* argv[])
{
	atexit(myexit);
	InitSock();

	SOCKADDR_IN			addr		= {0};
	int					nLen		= sizeof(addr);
	SOCKET				sock		= 0;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	addr.sin_family = AF_INET;
//  	addr.sin_port = htons(8022);
//  	addr.sin_addr.S_un.S_addr = inet_addr("222.76.216.88");
 	addr.sin_port = htons(8603);
 	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.64");

	// 同步，异步测试
	int			nType		= 0;
	u_long		uFlag		= 1;
	int			nSize		= sizeof(nType);

// 	getsockopt(sock, SOL_SOCKET, SO_OPENTYPE, (char*)&nType, &nSize);
	ioctlsocket(sock, FIONBIO, &uFlag);
// 	ioctlsocket(sock, FIONBIO, &uFlag);
// 	getsockopt(sock, SOL_SOCKET, SO_OPENTYPE, (char*)&nType, &nSize);
// 	uFlag = 0;
// 	ioctlsocket(sock, FIONBIO, &uFlag);
// 	getsockopt(sock, SOL_SOCKET, SO_OPENTYPE, (char*)&nType, &nSize);

	if( 0 != ConnectEx(sock, (SOCKADDR*)&addr, nLen) )
	{
		printf("连接222.76.216.88:8022失败: %d\n", WSAGetLastError());
		return 0;
	}
	// 接收数据
// 	CHAR		szData[1024]		= {0};
// 
// 	recv(sock, szData, 1024, 0);
// 	printf("Recv: %s\n", szData);
	// 开始聊天
	while(true)
	{
		char		szSend[1024]		= {0};

		printf("请输入发送内容: ");
		gets(szSend);
		if(strcmp(szSend, "exit") == 0)
			break;
		if( -1 == send(sock, szSend, (int)strlen(szSend), 0) )
			break;
		// 等等回应
		memset(szSend, 0, sizeof(szSend));
		if( -1 == recv(sock, szSend, sizeof(szSend), 0) )
			break;
		printf("server say: %s\r\n", szSend);
	}
	return 0;
}

void InitSock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		return;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return;
	}
}

void myexit()
{
	system("Pause");
}