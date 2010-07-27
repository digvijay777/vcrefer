// NatClient.cpp : implementation file
//

#include "stdafx.h"
#include "NetClient.h"
#include "NatClient.h"
#include ".\natclient.h"


// CNatClient dialog
CNatClient* CNatClient::s_pThis;

IMPLEMENT_DYNAMIC(CNatClient, CDialog)
CNatClient::CNatClient(CWnd* pParent /*=NULL*/)
	: CDialog(CNatClient::IDD, pParent)
{
	GetModuleFileName(NULL, m_strINI.GetBufferSetLength(1024), 1024);
	m_strINI.ReleaseBuffer();
	m_strINI = m_strINI.Left(m_strINI.ReverseFind('.'));
	m_strINI += ".ini";
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	s_pThis = this;
	m_sockChart = 0;
	m_svrSock = 0;
}

CNatClient::~CNatClient()
{
	if(INVALID_SOCKET != m_sock)
	{
		shutdown(m_sock, SD_BOTH);
		closesocket(m_sock);
	}
}

void CNatClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT2, m_chart);
	DDX_Control(pDX, IDC_EDIT1, m_send);
	DDX_Control(pDX, IDC_LIST2, m_usr);
}


BEGIN_MESSAGE_MAP(CNatClient, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BT_HOLE, OnBnClickedBtHole)
	ON_BN_CLICKED(IDC_BT_SEND, OnBnClickedBtSend)
END_MESSAGE_MAP()


// CNatClient message handlers

BOOL CNatClient::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	EnableChart(FALSE);
	EnableUser(FALSE);

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNatClient::OnBnClickedOk()
{
	// 连接服务器
	CHAR		szIP[256]		= {0};
	int			nPort			= 0;

	nPort = GetPrivateProfileInt("Nat", "Port", 6000, m_strINI.GetBuffer());
	GetPrivateProfileString("Nat", "IP", "222.76.216.87", szIP, sizeof(szIP), m_strINI.GetBuffer());

	SOCKADDR_IN			addr	= {0};
	int					nerr;
	CString				strmsg;

	addr.sin_addr.S_un.S_addr = inet_addr(szIP);
	addr.sin_port = htons(nPort);
	addr.sin_family = AF_INET;
	nerr = connect(m_sock, (SOCKADDR *)&addr, sizeof(addr));
	if(0 != nerr)
	{
		strmsg.Format("connect to %s:%d failed: %d", szIP, nPort, WSAGetLastError());
	}
	else
	{
		//send(m_sock, "", 3, 0);
		strmsg.Format("connect to %s:%d successed.", szIP, nPort);
		EnableChart(TRUE);
		
		send(m_sock, "G5", 3, 0);
		recv(m_sock, (char *)&m_svrSock, sizeof(m_svrSock), 0);
		CreateThread(NULL, 0, OnStartListen, NULL, 0, NULL);
	}

	m_list.AddString(strmsg);
}

void CNatClient::EnableChart(BOOL bChar /* = FALSE */)
{
	GetDlgItem(IDOK)->EnableWindow(!bChar);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bChar);
	m_usr.EnableWindow(bChar);
}

void CNatClient::EnableUser(BOOL bChar /* = FALSE */)
{
	GetDlgItem(IDC_BT_HOLE)->EnableWindow(bChar);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(bChar);
}

void CNatClient::OnBnClickedButton1()
{
	// 获取服务器的用户列表
	CHAR		szBuffer[1025]		= {0};
	int			nret;

	m_usr.ResetContent();
	send(m_sock, "G1", 3, 0);
	GetDlgItem(IDC_BT_HOLE)->EnableWindow(TRUE);
}

void CNatClient::OnBnClickedBtHole()
{
	SOCKET		sockRequest		= (SOCKET)m_usr.GetItemData(m_usr.GetCurSel());


	if(0 == sockRequest)
		return;

	SOCKET		sock;
	CHAR		szIP[256]		= {0};
	int			nPort			= 0;

	nPort = GetPrivateProfileInt("Nat", "Port", 6000, m_strINI.GetBuffer());
	GetPrivateProfileString("Nat", "IP", "222.76.216.87", szIP, sizeof(szIP), m_strINI.GetBuffer());

	SOCKADDR_IN			addr	= {0};
	int					nerr;
	CString				strmsg;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_addr.S_un.S_addr = inet_addr(szIP);
	addr.sin_port = htons(nPort);
	addr.sin_family = AF_INET;
	nerr = connect(sock, (SOCKADDR *)&addr, sizeof(addr));
	if(0 != nerr)
	{
		strmsg.Format("connect to %s:%d failed: %d", szIP, nPort, WSAGetLastError());
	}
	else
	{
		char		szCmd[10]		= {0};

		strcpy(szCmd, "F1");
		memcpy(&szCmd[2], &sockRequest, 4);
		memcpy(&szCmd[6], &m_svrSock, 4);
		send(sock, szCmd, 10, 0);

		int		len		= sizeof(addr);
		getsockname(sock, (SOCKADDR *)&addr, &len);
		closesocket(sock);

		m_usr.GetText(m_usr.GetCurSel(), m_strHoleIP);
		m_strHoleIP = m_strHoleIP.Left(m_strHoleIP.Find(':'));
		CreateThread(NULL, 0, OnStartHole, (LPVOID)htons(addr.sin_port), 0, NULL);
//		CreateThread(NULL, 0, OnConnectHole, (LPVOID)htons(addr.sin_port), 0, NULL);
	}

	m_list.AddString(strmsg);
}

DWORD CNatClient::OnStartHole(LPVOID lpParam)
{
	s_pThis->StartHole((int)lpParam);
	return 0;
}

DWORD CNatClient::OnStartListen(LPVOID lpParam)
{
	s_pThis->StartListen();
	return 0;
}

DWORD CNatClient::OnConnectHole(LPVOID lpParam)
{
	s_pThis->ConnectHole((INT)lpParam);
	return 0;
}
DWORD CNatClient::OnChart(LPVOID lpParam)
{
	s_pThis->Chart();
	return 0;
}

void CNatClient::StartHole(int nPort)
{
	SOCKET		sock;

	SOCKADDR_IN			addr	= {0};
	int					nerr;
	CString				strmsg;
	int					flag		= 1;
	int					len			= sizeof(flag);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);
	
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	addr.sin_port = htons(nPort);
	addr.sin_family = AF_INET;
	bind(sock, (SOCKADDR *)&addr, sizeof(addr));

	ULONG		argp		= 1;
	ioctlsocket(sock, FIONBIO, &argp);
	// 等待商口发过来
	addr.sin_addr.S_un.S_addr = inet_addr(m_strHoleIP.GetBuffer());
	WaitForSingleObject(m_hEvent, INFINITE);
	addr.sin_port = htons(m_nPortChart);
	strmsg.Format("start hole port: %d", m_nPortChart);
	m_list.AddString(strmsg);

	for(int i = 3000; i >= 0; i--)
	{
		nerr = connect(sock, (SOCKADDR *)&addr, sizeof(addr));

		struct timeval	LmtTime; 
		fd_set			SockList;
		int				nError;

		FD_ZERO(&SockList);
		FD_SET(sock, &SockList);

		LmtTime.tv_sec = 1; //连接超时: 秒 
		LmtTime.tv_usec = 0; 
		nerr = select(0, 0, &SockList, 0, &LmtTime); 
		if(nerr == 1)
		{
			m_sockChart = sock;
			argp = 0;
			ioctlsocket(sock, FIONBIO, &argp);
			GetDlgItem(IDC_BT_SEND)->EnableWindow(TRUE);
			m_list.AddString("Connect to desc.");
			CreateThread(NULL, 0, OnChart, NULL, 0, NULL);
		}
// 		if(0 != m_sockChart)
// 			break;
// 		Sleep(1);
	}	
}

void CNatClient::ConnectHole(int nPort)
{
	SOCKET		sock;
	CHAR		szIP[256]		= {0};

	SOCKADDR_IN			addr	= {0};
	int					nerr;
	CString				strmsg;
	int					flag		= 1;
	int					len			= sizeof(flag);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	addr.sin_port = htons(nPort);
	addr.sin_family = AF_INET;
	bind(sock, (SOCKADDR *)&addr, sizeof(addr));

	listen(sock, 5);

	while(true)
	{
		SOCKET				client;
		SOCKADDR_IN			addrin;
		int					len			= sizeof(addrin);

		client = accept(sock, (SOCKADDR *)&addrin, &len);
		
		if(INVALID_SOCKET != client)
		{
			m_sockChart = client;
			GetDlgItem(IDC_BT_SEND)->EnableWindow(TRUE);
			m_list.AddString("accept src connect.");
			CreateThread(NULL, 0, OnChart, NULL, 0, NULL);
			/*break;*/
		}
	}

	closesocket(sock);
}

// 开始监听
void CNatClient::StartListen()
{
	SOCKADDR_IN		addr;
	int				nLen				= sizeof(addr);
	CHAR			szBuffer[1024];
	CHAR			szCmd[1024];
	CString			strMsg;
	int				nerr;

	while(true)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		nerr = recv(m_sock, szBuffer, sizeof(szBuffer), 0);
		if(nerr <= 0)
			break; // 出错
		if(WSAENOTCONN == WSAGetLastError())
			break;
		// 处理命令
		unsigned short		nCmd		= *((unsigned short *)&szBuffer[0]);

		if('2F' == nCmd)
		{
			// 请求对方打洞
			SOCKET*				pDessock		= (SOCKET *)&szBuffer[2];
			SOCKADDR_IN*		pSendaddr		= (SOCKADDR_IN*)&szBuffer[6];

			m_nPortChart = ntohs(pSendaddr->sin_port);
			SetEvent(m_hEvent);
			strMsg.Format("Get hole port: %d", m_nPortChart);
			m_list.AddString(strMsg);
			
			// 发送请求
			SOCKET		sock;
			CHAR		szIP[256]		= {0};
			int			nPort			= 0;

			nPort = GetPrivateProfileInt("Nat", "Port", 6000, m_strINI.GetBuffer());
			GetPrivateProfileString("Nat", "IP", "222.76.216.87", szIP, sizeof(szIP), m_strINI.GetBuffer());

			SOCKADDR_IN			addr	= {0};
			int					nerr;
			CString				strmsg;

			sock = socket(AF_INET, SOCK_STREAM, 0);
			addr.sin_addr.S_un.S_addr = inet_addr(szIP);
			addr.sin_port = htons(nPort);
			addr.sin_family = AF_INET;
			nerr = connect(sock, (SOCKADDR *)&addr, sizeof(addr));
			if(0 != nerr)
			{
				strmsg.Format("connect to %s:%d failed: %d", szIP, nPort, WSAGetLastError());
			}
			else
			{
				char		szCmd[10]		= {0};

				strcpy(szCmd, "F3");
				memcpy(&szCmd[2], pDessock, 4);
				send(sock, szCmd, 6, 0);

				int		len		= sizeof(addr);
				getsockname(sock, (SOCKADDR *)&addr, &len);
				closesocket(sock);

				m_strHoleIP = inet_ntoa(pSendaddr->sin_addr);
				CreateThread(NULL, 0, OnStartHole, (LPVOID)htons(addr.sin_port), 0, NULL);
				CreateThread(NULL, 0, OnConnectHole, (LPVOID)htons(addr.sin_port), 0, NULL);
			}

			m_list.AddString(strmsg);

			break;
		}
		else if('4F' == nCmd)
		{
			// 回应对方打漏洞
			SOCKET*				pDessock		= (SOCKET *)&szBuffer[2];
			SOCKADDR_IN*		pSendaddr		= (SOCKADDR_IN*)&szBuffer[6];

			m_nPortChart = ntohs(pSendaddr->sin_port);
			strMsg.Format("Request hole port: %d", m_nPortChart);
			m_list.AddString(strMsg);
			SetEvent(m_hEvent);

			break;
		}
		else if('2G' == nCmd)
		{
			m_list.ResetContent();
			for(int i = 0; i < 200; i++)
			{
				CHAR		ip[1024]		= {0};

				if(0 == *((DWORD *)&szBuffer[i*4+2]))
					break;

				strcpy(ip, "G3");
				memcpy(&ip[2], &szBuffer[i*4+2], 4);
				send(m_sock, ip, 7, 0);
				memset(ip, 0, sizeof(ip));
				recv(m_sock, ip, sizeof(ip), 0);
				SOCKADDR_IN*		paddr		= (SOCKADDR_IN *)&ip[2];
				CString			strUser;
				strUser.Format("%s:%d", inet_ntoa(paddr->sin_addr), (int)ntohs(paddr->sin_port));
				int n = m_usr.AddString(strUser);
				m_usr.SetItemData(n, (DWORD_PTR)*((SOCKET *)&szBuffer[i*4+2]));
			}
		}
	}
}
void CNatClient::OnBnClickedBtSend()
{
	CString			strText;

	m_send.GetWindowText(strText);
	send(m_sockChart, strText.GetBuffer(), strText.GetLength()+1, 0);
}

VOID CNatClient::Chart()
{
	CString			strText;

	while(true)
	{
		recv(m_sockChart, strText.GetBufferSetLength(1024), 1024, 0);
		strText.ReleaseBuffer();
		Sleep(1);

		if("" == strText)
			continue;

		strText+= "\r\n";
		m_chart.SetSel(-1, -2);
		m_chart.ReplaceSel(strText.GetBuffer());
	}
}