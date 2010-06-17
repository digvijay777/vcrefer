#pragma once

#include <WinSock2.h>

#pragma pack(push)
#pragma pack(1)

// ���󷽷�
enum SOCKSMETHOD{};

// ������������ݽṹ


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

// ���������汾
#define PROXY_ERROR_VERSION					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x400)

// ��Ч���û���/����
#define PROXY_ERROR_USER					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x401)

// ���ǿɽ��յķ�ʽ
#define PROXY_ERROR_INVALID_METHOD			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x402)

// �ܾ�ת��
#define PROXY_ERROR_ALLOW					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x403)
