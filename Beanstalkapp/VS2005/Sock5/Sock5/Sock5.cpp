// Sock5.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include "Sock5Client.h"
#include <process.h>

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
 	addr.sin_port = htons(8022);
 	addr.sin_addr.S_un.S_addr = inet_addr("222.76.216.88");

	if( 0 != ConnectEx(sock, (SOCKADDR*)&addr, nLen) )
	{
		printf("连接222.76.216.88:8022失败: %d\n", WSAGetLastError());
		return 0;
	}
	// 接收数据
	CHAR		szData[1024]		= {0};

	recv(sock, szData, 1024, 0);
	printf("Recv: %s\n", szData);

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