#pragma once

/* 兼容代理服务器的连接 */
int __stdcall ConnectEx(SOCKET s, const struct sockaddr* name, int namelen);
/* 获取代理设置 */
bool __stdcall GetSockInfo(int* nType, char* pAddr, int* nPort, char* pUser, char* pPwd);
/* 通过Sock4连接 */
int __stdcall ConnectFromSock4(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen);
/* 通过Sock5连接 */
int __stdcall ConnectFromSock5(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen, const char* pUser, const char* pPwd);
/* 通过HTTP代理协议 */
int __stdcall ConnectFromHttp(SOCKET s, const struct sockaddr* name, int namelen, const struct sockaddr* sockname, int nLen);
/* 打开注册表 */
long __stdcall OpenIEProxyKey(HKEY* pKey);