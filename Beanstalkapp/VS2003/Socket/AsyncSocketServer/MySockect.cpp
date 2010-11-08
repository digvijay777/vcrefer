#include "mysocket.h"

CMySock*		g_pSocket	= NULL;
std::vector<CClientSock*> g_vctClient;

BOOL CreateSockeServer()
{
	ASSERT(NULL == g_pSocket);
	g_pSocket = new CMySock();
	if(NULL == g_pSocket)
	{
		printf("分配内存失败");
		return FALSE;
	}
	EXCUTEFALSEFUN(g_pSocket->Create(8603), "创建SOCKET失败");
	EXCUTEFALSEFUN(g_pSocket->Listen(), "监听失败");
	return TRUE;
}
LRESULT CallSocketProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CMySock::CMySock(int nMaxCon)
{
	m_nConnectNum = nMaxCon;
}

CMySock::~CMySock()
{

}

void CMySock::OnAccept(int nErrorCode)
{
	CClientSock*	pClient = new CClientSock();
	if(!Accept(*pClient))
	{
		delete pClient;
		printf("Accept fail!\n");
		return;
	}
	if(g_vctClient.size() > m_nConnectNum)
	{
		printf("连接过多\n");
		pClient->Close();
		return;
	}
	g_vctClient.push_back(pClient);
	//发送欢迎信息
	char			buff[100]	= {0};
	SOCKADDR_IN		sClient[10];
	int				nLength		= sizeof(SOCKADDR_IN) * 10;
	if(pClient->GetSockName((SOCKADDR *)sClient, &nLength))
	{
		strcpy(pClient->m_szIP, inet_ntoa(sClient[0].sin_addr));
		sprintf(buff, "Welcome %s!", inet_ntoa(sClient[0].sin_addr));
		printf("a client accept:%s %d\n", inet_ntoa(sClient[0].sin_addr), (UINT)pClient);
	}
	else
	{
		strcpy(buff, "Welcome!");
		printf("a client accept:%d\n", (UINT)pClient);
	}
	pClient->Send(buff, strlen(buff) + 1);
}

//////////////////////////////////////////////////////////////////////////
CClientSock::CClientSock()
{
	memset(m_szIP, 0, 100);
}

CClientSock::~CClientSock()
{

}

void CClientSock::OnReceive(int nErrorCode)
{
	char		buff[100]	= {0};

	if(Receive(buff, 100))
	{
		if(stricmp(buff, "quit") == 0)
		{
			std::vector<CClientSock *>::iterator iter = std::find(g_vctClient.begin(), g_vctClient.end(), this);
			if(g_vctClient.end() != iter)
				g_vctClient.erase(iter);
			printf("msg %s:%d closed!\n", m_szIP, (unsigned int)this);
			Send("quit", 5);
			this->Close();
			delete this;
			return;
		}
		printf("receive %s:%d say:%s\n", m_szIP, (unsigned int)this, buff);
		Send("requested!", strlen("requested!") + 1);
	}
	else
	{
		printf("receive %s:%d failed!\n", m_szIP, (unsigned int)this);
	}
}

void CClientSock::OnClose(int nErrorCode)
{
	std::vector<CClientSock *>::iterator iter = std::find(g_vctClient.begin(), g_vctClient.end(), this);
	if(g_vctClient.end() != iter)
		g_vctClient.erase(iter);
	printf("msg %s:%d closed!\n", m_szIP, (unsigned int)this);
	this->Close();
	delete this;
}