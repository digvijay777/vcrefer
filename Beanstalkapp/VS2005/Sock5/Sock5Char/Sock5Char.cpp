// Sock5Char.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <process.h> 
#include <Winsock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main(int, char **, char **)
{
	//////////////////////////////////////////////////////////////////////////
	// 初始化工作 
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		return err;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	// 通信操作
	SOCKET			server		= {0};
	SOCKADDR_IN		addr		= {0};
	int				nRet		= 0;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET != server)
	{
		do 
		{
			addr.sin_port = htons(8603);
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = /*inet_addr("161.250.41.200")*/htonl(INADDR_ANY);
			//梆定
			nRet = bind(server, (SOCKADDR *)&addr, sizeof(SOCKADDR));
			if(0 != nRet)
			{
				printf("bind error:%d\n", WSAGetLastError());
				break;
			}
			//监听
			nRet = listen(server, 5);
			if(0 != nRet)
			{
				printf("listen error:%d\n", WSAGetLastError());
				break;
			}
			//接收
			while(1)
			{
				SOCKADDR_IN		cAddr		= {0};
				int				cLen		= sizeof(SOCKADDR);
				SOCKET			client		= {0};
				//阻塞
				client = accept(server, (SOCKADDR *)&cAddr, &cLen);
				if(INVALID_SOCKET == client)
					continue;
				// 开始聊天
				char			szIP[128]		= {0};

				strcpy(szIP, inet_ntoa(cAddr.sin_addr));
				printf("与%s建立连接\r\n", szIP);
				while(true)
				{
					char		szRecv[1024]		= {0};
					
					// 接收
					if( -1 == recv(client, szRecv, sizeof(szRecv), 0) )
						break;
					printf("'%s' say: %s\r\n", szIP, szRecv);
					// 等待输入
					memset(szRecv, 0, sizeof(szRecv));
					printf("请输入回应: ");
					gets(szRecv);
					// 发送
					if( -1 == send(client, szRecv, (int)strlen(szRecv), 0) )
						break;
				}
				printf("与%s断开连接\r\n", szIP);
				closesocket(client);
			}
		} while (0);
		closesocket(server);
	}
	else
	{
		printf("socket error:%d\n", WSAGetLastError());
	}

	//////////////////////////////////////////////////////////////////////////
	// 清理工作
	WSACleanup();
	system("pause");
	return 0;
}
// int _tmain(int argc, _TCHAR* argv[])
// {
// 	return 0;
// }

