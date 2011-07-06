#pragma once
#include <WinSock2.h>
#include <Windows.h>

#pragma pack(push,1)
// ����ͷ���ṹ
struct CmdHeader
{
	DWORD		dwVersion;	// ��������汾
	DWORD		dwSize;		// �˴δ����ܴ�С
	CHAR		szCmd[32];	// ��������, �˺�Ϊʵ�ʵĴ�������
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
