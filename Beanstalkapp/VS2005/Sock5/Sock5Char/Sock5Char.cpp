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
	// ��ʼ������ 
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
	// ͨ�Ų���
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
			//��
			nRet = bind(server, (SOCKADDR *)&addr, sizeof(SOCKADDR));
			if(0 != nRet)
			{
				printf("bind error:%d\n", WSAGetLastError());
				break;
			}
			//����
			nRet = listen(server, 5);
			if(0 != nRet)
			{
				printf("listen error:%d\n", WSAGetLastError());
				break;
			}
			//����
			while(1)
			{
				SOCKADDR_IN		cAddr		= {0};
				int				cLen		= sizeof(SOCKADDR);
				SOCKET			client		= {0};
				//����
				client = accept(server, (SOCKADDR *)&cAddr, &cLen);
				if(INVALID_SOCKET == client)
					continue;
				// ��ʼ����
				char			szIP[128]		= {0};

				strcpy(szIP, inet_ntoa(cAddr.sin_addr));
				printf("��%s��������\r\n", szIP);
				while(true)
				{
					char		szRecv[1024]		= {0};
					
					// ����
					if( -1 == recv(client, szRecv, sizeof(szRecv), 0) )
						break;
					printf("'%s' say: %s\r\n", szIP, szRecv);
					// �ȴ�����
					memset(szRecv, 0, sizeof(szRecv));
					printf("�������Ӧ: ");
					gets(szRecv);
					// ����
					if( -1 == send(client, szRecv, (int)strlen(szRecv), 0) )
						break;
				}
				printf("��%s�Ͽ�����\r\n", szIP);
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
	// ������
	WSACleanup();
	system("pause");
	return 0;
}
// int _tmain(int argc, _TCHAR* argv[])
// {
// 	return 0;
// }

