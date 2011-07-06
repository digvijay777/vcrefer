// First.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <wsnwlink.h>
#include <wsipx.h>
// http://topic.csdn.net/t/20020623/15/824335.html

#pragma comment(lib, "Ws2_32.lib")

void		DoWorkServer(int argc, _TCHAR* argv[]);
void		DoWorkClient(int argc, _TCHAR* argv[]);

int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
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
			return 0; 
	}

	/* The WinSock DLL is acceptable. Proceed. */
	if(argc > 1 && _tcsicmp(argv[1], _T("-client")) == 0)
	{
		DoWorkClient(argc, argv);
	}
	else
	{
		DoWorkServer(argc, argv);
	}

	WSACleanup( );
	return 0;
}

void DoWorkServer(int argc, _TCHAR* argv[])
{
	SOCKET				sock		= NULL;
	SOCKADDR_IPX		addr		= {0};
	int					err			= 0;

	sock = socket(AF_IPX, SOCK_STREAM, NSPROTO_SPX);
	if(INVALID_SOCKET == sock)
	{
		printf("无法创建套接字: %d\n", WSAGetLastError());
		return;
	}
	
	addr.sa_family = AF_IPX;
	addr.sa_socket = htons(8603);
	err = bind(sock, (SOCKADDR *)&addr, sizeof(addr));
	if(0 != err)
	{
		printf("无法绑定SOCKET: %d\n", err);
		return ;
	}

	listen(sock, 5);

	printf("开始监听\n");
	while(1)
	{
		SOCKET			client			= NULL;
		SOCKADDR_IPX	caddr			= {0};
		int				nLen			= sizeof(caddr);

		client = accept(sock, (SOCKADDR *)&caddr, &nLen);
		// 开始接收
		CHAR			szRecv[1024]	= {0};

		recv(client, szRecv, sizeof(szRecv), 0);
		printf("%s,%s: %s\n", caddr.sa_netnum, caddr.sa_nodenum, szRecv);
		// 发送
		send(client, "hello", 6, 0);
		// 关闭
		closesocket(client);
	}

	closesocket(sock);
}

void DoWorkClient(int argc, _TCHAR* argv[])
{
	SOCKET				sock		= NULL;
	SOCKADDR_IPX		addr		= {0};
	int					err			= 0;

	sock = socket(AF_IPX, SOCK_STREAM, NSPROTO_SPX);
	if(INVALID_SOCKET == sock)
	{
		printf("无法创建套接字: %d\n", WSAGetLastError());
		return;
	}

	addr.sa_family = AF_IPX;
	addr.sa_socket = htons(8603);
	//connect()

	closesocket(sock);
}