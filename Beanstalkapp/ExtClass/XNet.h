#pragma once

namespace XNet
{
	BOOL		ConnectWidthProxy4(SOCKET sock, LPCSTR lpDesAddr, int nDesPort, LPCSTR lpProxyAddr, int nProxyPort);
	BOOL		ConnectWidthProxy5(SOCKET sock, LPCSTR lpDesAddr, int nDesPort, LPCSTR lpProxyAddr, int nProxyPort, LPCSTR lpUser = NULL, LPCSTR lpPasswd = NULL);
	BOOL		SendProxy5Query(SOCKET sock, LPCSTR lpUser, LPCSTR lpPasswd);
	BOOL		LoginProxy5(SOCKET sock, LPCSTR lpUser = NULL , LPCSTR lpPasswd = NULL );
	ULONG		GetInetAddr(LPCSTR lpAddr); 
}