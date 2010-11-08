#include <afxsock.h>
#include <vector>
#include <algorithm>

#define UM_SOCKET		(WM_USER+1)

#define EXCUTEFALSEFUN(fun,strmsg) \
	if(FALSE == (fun))\
	{\
		printf(strmsg##"\n");\
		return FALSE;\
	}

BOOL CreateSockeServer();

class CMySock : public CAsyncSocket
{
public:
	CMySock(int nMaxCon = 5);
	~CMySock();
private:
	int		m_nConnectNum;
public:
	virtual void OnAccept(int nErrorCode);
};

class CClientSock : public CAsyncSocket
{
public:
	CClientSock();
	~CClientSock();
private:
	char		m_szIP[100];
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
public:
	friend CMySock;
};



extern std::vector<CClientSock*> g_vctClient;
extern CMySock* g_pSocket;
