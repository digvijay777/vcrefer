#pragma once
#include <vector>

#pragma pack(push, 1)

/*
 *  接收格式
 *  
 *   +----+----+----+----+----+----+----+----+----+----+...+----+
 *   | VN | CD | DSTPORT |      DSTIP        | USERID      |NULL|
 *   +----+----+----+----+----+----+----+----+----+----+...+----+
 *      1    1      2              4           variable       1
 */
typedef struct tagProxyRequest4
{
	BYTE			vn;				// 版本
	BYTE			nCommand;		// 命令 见后面的定义
	USHORT			nDesPort;		// 目标端口号
	ULONG			nDesIP;			// 目标IP
	CHAR			szUserIP[1];	// 用户ID
}PROXY_REQUEST4;

// COMMAND定义
#define PROXY_COMMAND_CONNECT			0x1
#define PROXY_COMMAND_BIND				0x2

/*
 *  回应格式 
 *  
 *   +----+----+----+----+----+----+----+----+
 *   | VN | CD | DSTPORT |      DSTIP        |
 *   +----+----+----+----+----+----+----+----+
 *      1    1      2              4
 */
typedef struct tagProxyResponse4
{
	BYTE		vn;			// 版本 0x4
	BYTE		nResult;	// 结果，见后定义
}PROXY_RESPONSE4;

// 回应结果
#define PROXY_RESPONSE_SUCCESS			0x5A		// 允许转发
#define PROXY_RESPONSE_FAILED			0x5B		// 拒绝转发，一般性失败
#define PROXY_RESPONSE_INVALIDCONNECT	0x5C		// 拒绝转发，SOCKS 4 Server无法连接到SOCS 4 Client所在主机的IDENT服务
#define PROXY_RESPONSE_ERRORUSER		0x5D		// 拒绝转发，请求报文中的USERID与IDENT服务返回值不相符

// 类
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
		char				szRawBuff[1024];		// 真实数据
		SOCKET				sock;					// SOCKET
	};
	
	WSAOVERLAPPEDEX		WSARequest;			// 请求者 
	WSAOVERLAPPEDEX		WSAConnect;			// 连接者

	BYTE				nProxyVersion;		// 代理协议版本

}WSAOVERLAPPEDPROXY, *LPWSAOVERLAPPEDPROXY;

// 启动服务
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