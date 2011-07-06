// BroadCastServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BroadCastSvr.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CBroadCastSvr::SocketStartup(2, 2);

	CBroadCastSvr::DoListen(4040);

	CBroadCastSvr::SocketCleanup();
	return 0;
}

