#include "StdAfx.h"
#include "BroadCastSvr.h"

#pragma comment(lib, "Ws2_32.lib")

CBroadCastSvr::CBroadCastSvr(void)
{
}

CBroadCastSvr::~CBroadCastSvr(void)
{
}

//////////////////////////////////////////////////////////////////////////
bool CBroadCastSvr::SocketStartup(WORD wl /* = 1 */, WORD wh /* = 1 */)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( wl, wh );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		fprintf(stderr, "[WSAStartup] error: %d\n", err);
		return false;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != wl ||
		HIBYTE( wsaData.wVersion ) != wh ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			fprintf(stderr, "[WSAStartup] error version(%d, %d).\n", (int)wl, (int)wh);
			return false; 
	}
	
	return true;
}

bool CBroadCastSvr::SocketCleanup()
{
	WSACleanup( );
	return true;
}
// 监听
void CBroadCastSvr::DoListen(int nPort)
{
	SOCKET			sock		= NULL;
	SOCKADDR_IN		addr		= {0};

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(NULL == sock || INVALID_SOCKET == sock)
	{
		fprintf(stderr, "无法创建SOCKET: %d\n", WSAGetLastError());
		return;
	}
	// 监听
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr = ADDR_ANY;
	// 绑定
	bind(sock, (SOCKADDR *)&addr, sizeof(addr));
	// 
}