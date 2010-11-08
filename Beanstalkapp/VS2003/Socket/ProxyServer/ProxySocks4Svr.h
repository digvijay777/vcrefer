#pragma once
#include <vector>

#pragma pack(push, 1)

/*
 *  ���ո�ʽ
 *  
 *   +----+----+----+----+----+----+----+----+----+----+...+----+
 *   | VN | CD | DSTPORT |      DSTIP        | USERID      |NULL|
 *   +----+----+----+----+----+----+----+----+----+----+...+----+
 *      1    1      2              4           variable       1
 */
typedef struct tagProxyRequest4
{
	BYTE			vn;				// �汾
	BYTE			nCommand;		// ���� ������Ķ���
	USHORT			nDesPort;		// Ŀ��˿ں�
	ULONG			nDesIP;			// Ŀ��IP
	CHAR			szUserIP[1];	// �û�ID
}PROXY_REQUEST4;

// COMMAND����
#define PROXY_COMMAND_CONNECT			0x1
#define PROXY_COMMAND_BIND				0x2

/*
 *  ��Ӧ��ʽ 
 *  
 *   +----+----+----+----+----+----+----+----+
 *   | VN | CD | DSTPORT |      DSTIP        |
 *   +----+----+----+----+----+----+----+----+
 *      1    1      2              4
 */
typedef struct tagProxyResponse4
{
	BYTE		vn;			// �汾 0x4
	BYTE		nResult;	// �����������
}PROXY_RESPONSE4;

// ��Ӧ���
#define PROXY_RESPONSE_SUCCESS			0x5A		// ����ת��
#define PROXY_RESPONSE_FAILED			0x5B		// �ܾ�ת����һ����ʧ��
#define PROXY_RESPONSE_INVALIDCONNECT	0x5C		// �ܾ�ת����SOCKS 4 Server�޷����ӵ�SOCS 4 Client����������IDENT����
#define PROXY_RESPONSE_ERRORUSER		0x5D		// �ܾ�ת�����������е�USERID��IDENT���񷵻�ֵ�����

// ��
class CProxySocks4Svr
{
public:
	CProxySocks4Svr(void);
	virtual ~CProxySocks4Svr(void);

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef struct tagWSAOVERLAPPEDPROXY
{
	struct WSAOVERLAPPEDEX : public WSAOVERLAPPED
	{
		WSABUF				WSABuf;
		char				szRawBuff[1024];		// ��ʵ����
		SOCKET				sock;					// SOCKET
	};
	
	WSAOVERLAPPEDEX		WSARequest;			// ������ 
	WSAOVERLAPPEDEX		WSAConnect;			// ������

	BYTE				nProxyVersion;		// ����Э��汾

}WSAOVERLAPPEDPROXY, *LPWSAOVERLAPPEDPROXY;

// ��������
class CSocksServer
{
public:
	CSocksServer();
	virtual ~CSocksServer();
public:
	int StartServer(int nPort = 1080);
	void Stop();

public:
	HANDLE			m_hThreadAccept;
private:
	HANDLE			m_hCompetionPort;
	SOCKET			m_sockSvr;
	int				m_nPort;
	BOOL			m_bRunning;
	std::vector<LPWSAOVERLAPPEDPROXY>		m_vctWSAOL;
public:
	static DWORD _stdcall	ThreadAccept(LPVOID lpParameter);
	static DWORD _stdcall	ThreadWork(LPVOID lpParameter);
protected:
	void CloseSocket(LPWSAOVERLAPPEDPROXY lpOvlp, SOCKET sock);
	virtual DWORD	OnAccept();
	virtual DWORD	OnWork();
};
#pragma pack(pop)