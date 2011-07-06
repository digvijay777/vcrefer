#pragma once
#include <WinSock2.h>
#include <Windows.h>

#pragma pack(push,1)
// 命令头部结构
struct CmdHeader
{
	DWORD		dwVersion;	// 传输命令版本
	DWORD		dwSize;		// 此次传输总大小
	CHAR		szCmd[32];	// 命令名字, 此后为实际的传输数据
};

#pragma pack(pop)

class CBroadCastSvr
{
public:
	CBroadCastSvr(void);
	~CBroadCastSvr(void);
public:
	static bool		SocketStartup(WORD wl = 1, WORD wh = 1);
	static bool		SocketCleanup();
	static void		DoListen(int nPort);
};
