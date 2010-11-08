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
			addr.sin_addr.S_un.S_addr = inet_addr("161.250.41.200")/*htonl(INADDR_ANY)*/;
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
				SOCKADDR_IN		cAddr	= {0};
				int				cLen	= sizeof(SOCKADDR);
				SOCKET			client	= {0};
				//阻塞
				client = accept(server, (SOCKADDR *)&cAddr, &cLen);
				if(INVALID_SOCKET == client)
					continue;
				char sendbuf[100];
				memset(sendbuf, 0, 100);
				printf("%s connect successed.\n", inet_ntoa(cAddr.sin_addr));
				sprintf(sendbuf, "Welcome %s to yuanjie.xia!", inet_ntoa(cAddr.sin_addr));
				send(client, sendbuf, strlen(sendbuf) + 1, 0);
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