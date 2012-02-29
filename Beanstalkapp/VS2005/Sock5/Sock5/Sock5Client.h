#pragma once

/* ���ݴ�������������� */
int __stdcall ConnectEx(SOCKET s, const struct sockaddr* name, int namelen);
/* ��ȡ�������� */
bool __stdcall GetSockInfo(int* nType, char* pAddr, int* nPort, char* pUser, char* pPwd);
/* ͨ��Sock4���� */
int __stdcall ConnectFromSock4(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen);
/* ͨ��Sock5���� */
int __stdcall ConnectFromSock5(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen, const char* pUser, const char* pPwd);
/* ͨ��HTTP����Э�� */
int __stdcall ConnectFromHttp(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen);
/* ��ע��� */
long __stdcall OpenIEProxyKey(HKEY* pKey);