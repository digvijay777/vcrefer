#include "stdio.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int Setup_for_ping(char* host, int ttl, SOCKET& sd, SOCKADDR_IN& dest);
int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		perror("参数格式不正确\n");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	WSAData		wsaData;
	if(WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		perror("Failed to find Winsock 2.1 or better.\n");
		return 1;
	}
	do 
	{
		SOCKET			sd;
		SOCKADDR_IN		dest, source;
		int				ttl;
		if(Setup_for_ping(argv[1], ttl, sd, dest) < 0)
			break;
	} while (0);

	WSACleanup();
	system("pause");
}

int Setup_for_ping(char* host, int ttl, SOCKET& sd, SOCKADDR_IN& dest)
{
	char	szErr[1024];

	sd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 0, 0);
	if(INVALID_SOCKET == sd)
	{
		sprintf(szErr, "Failed to create raw socket:%d\n", WSAGetLastError());
		perror(szErr);
		return -1;
	}

	// Set the Created Socket
	if(setsockopt(sd, IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) == SOCKET_ERROR)
	{
		sprintf(szErr, "TTL setsockopt failed:%d\n", WSAGetLastError());
		perror(szErr);
		return -1;
	}

	// Init Desc address data
	memset(&dest, 0, sizeof(dest));

	unsigned int addr = inet_addr(host);
	if(INADDR_NONE != addr)
	{
		dest.sin_addr.s_addr = addr;
		dest.sin_family = AF_INET;
	}
	else
	{
		// not number
		hostent* hp = gethostbyname(host);
		if(hp != 0)
		{
			memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);
			dest.sin_family = hp->h_addrtype;
		}
		else
		{
			sprintf(szErr, "Failed to resolve %s\n", host);
			perror(szErr);
			return -1;
		}
	}

	return 0;
}