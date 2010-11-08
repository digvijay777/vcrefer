#pragma once

#include <WinSock2.h>

#pragma pack(push)
#pragma pack(1)

// 请求方法
enum SOCKSMETHOD{};

// 请求服务器数据结构


class CXSocksProxy
{
public:
	CXSocksProxy(int nVersion = 5, int nSendTimeout = -1, int nRecvTimeout = -1);
	~CXSocksProxy();

public:
	SOCKET			m_hSocket;

private:
	int				m_nVersion;
	int				m_nSendTimeout;
	int				m_nRecvTimeout;
public:
	BOOL		Connect(LPCSTR lpIP, int nPort, LPCSTR lpUser = NULL, LPCSTR lpPasswd = NULL);
	BOOL		Close();

	BOOL		ProxySocks4(LPCSTR lpDesIP, int nPort);
	BOOL		ProxySocks5(LPCSTR lpDesIP, int nPort, BOOL bByIP = TRUE);
private:
	BOOL		SendSocks5Query(LPCSTR lpUser = NULL, LPCSTR lpPasswd = NULL);
	BOOL		LogonSocks5(LPCSTR lpUser = NULL, LPCSTR lpPasswd = NULL);
};


#pragma pack(pop)

// 错误的请求版本
#define PROXY_ERROR_VERSION					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x400)

// 无效的用户名/密码
#define PROXY_ERROR_USER					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x401)

// 不是可接收的方式
#define PROXY_ERROR_INVALID_METHOD			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x402)

// 拒绝转发
#define PROXY_ERROR_ALLOW					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x403)
