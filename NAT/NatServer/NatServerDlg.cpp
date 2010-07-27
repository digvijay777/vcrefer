// NatServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NatServer.h"
#include "NatServerDlg.h"
#include ".\natserverdlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CNatServerDlg 对话框

CNatServerDlg*		CNatServerDlg::m_pThis;


CNatServerDlg::CNatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNatServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GetModuleFileName(NULL, m_strINI.GetBufferSetLength(1024), 1024);
	m_strINI.ReleaseBuffer();
	m_strINI = m_strINI.Left(m_strINI.ReverseFind('.'));
	m_strINI += ".ini";
	m_pThis = this;
}

void CNatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CNatServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CNatServerDlg 消息处理程序

BOOL CNatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	HANDLE		hThread;

	hThread = CreateThread(NULL, 0, OnThreadListen, 0, 0, NULL);
	CloseHandle(hThread);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CNatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNatServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CNatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNatServerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
}

DWORD CNatServerDlg::OnThreadListen(LPVOID lpParam)
{
	m_pThis->OnListen();

	return 0;
}

DWORD CNatServerDlg::OnThreadAccept(LPVOID lpParam)
{
	SOCKET		sock	= (SOCKET)lpParam;

	m_pThis->OnAccept(sock);	
	return 0;
}

void CNatServerDlg::OnAccept(SOCKET sock)
{
	m_vctClient.push_back(sock);

	SOCKADDR_IN		addr;
	int				nLen				= sizeof(addr);
	CHAR			szBuffer[1024];
	CHAR			szCmd[1024];
	CString			strMsg;
	int				nerr;

	// 用户登录
	getpeername(sock, (SOCKADDR *)&addr, &nLen);

	strMsg.Format("%s:%d log in.", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port));
	m_list.AddString(strMsg);

	while(true)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		nerr = recv(sock, szBuffer, sizeof(szBuffer), 0);
		if(nerr <= 0)
			break; // 出错
		if(WSAENOTCONN == WSAGetLastError())
			break;
		// 处理命令
		unsigned short		nCmd		= *((unsigned short *)&szBuffer[0]);
		
		if('0F' == nCmd)
		{
			// 登录
		}
		else if('FF' == nCmd)
		{
			// 登出
			break;
		}
		else if('1G' == nCmd)
		{
			// 获取用户列表
			memset(szCmd, 0, sizeof(szCmd));
			strcpy(szCmd, "G2");
			for(int i = 0; i < m_vctClient.size() && i < 200; i++)
			{
				memcpy(&szCmd[i*4+2], &m_vctClient[i], 4);
			}

			send(sock, szCmd, sizeof(szCmd), 0);
			strMsg.Format("%s get user list.", inet_ntoa(addr.sin_addr));
			m_list.AddString(strMsg);
		}
		else if('3G' == nCmd)
		{
			// 获取详细信息
			memset(szCmd, 0, sizeof(szCmd));
			SOCKET*			pSock			= (SOCKET *)&szBuffer[2];
			SOCKADDR_IN		sendaddr		= {0};
			int				nsendlen		= sizeof(sendaddr);

			strcpy(szCmd, "G4");
			if(m_vctClient.end() == find(m_vctClient.begin(), m_vctClient.end(), *pSock))
			{
				strcpy(&szCmd[2], "0.0.0.0");
			}
			else
			{
				getpeername(*pSock, (SOCKADDR *)&sendaddr, &nsendlen);
				memcpy(&szCmd[2], &sendaddr, sizeof(sendaddr));
			}

			send(sock, szCmd, sizeof(sendaddr) + 2, 0);
			strMsg.Format("%s get socket info %s.", inet_ntoa(addr.sin_addr), inet_ntoa(sendaddr.sin_addr));
			m_list.AddString(strMsg);
		}
		else if('5G' == nCmd)
		{
			send(sock, (char *)&sock, sizeof(sock), 0);
		}
		else if('1F' == nCmd)
		{
			// 请求对方打洞
			SOCKET*		pDessock				= (SOCKET *)&szBuffer[2];

			shutdown(sock, SD_BOTH);
			if(m_vctClient.end() == find(m_vctClient.begin(), m_vctClient.end(), *pDessock))
				break;

			// 发送请求
			SOCKADDR_IN		sendaddr		= {0};

			sendaddr.sin_family = AF_INET;
			sendaddr.sin_port = addr.sin_port;
			sendaddr.sin_addr = addr.sin_addr;
			memset(szCmd, 0, sizeof(szCmd));
			strcpy(szCmd, "F2");
			memcpy(&szCmd[2], &szBuffer[6], sizeof(sock));
			memcpy(&szCmd[6], &sendaddr, sizeof(sendaddr));
			send(*pDessock, szCmd, sizeof(sendaddr) + 6, 0);

			SOCKADDR_IN		ad1;
			int				nlen1		= sizeof(ad1);
			getpeername(*pDessock, (SOCKADDR *)&ad1, &nlen1);
			strMsg.Format("%s:%d request %s start hole.", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port), inet_ntoa(ad1.sin_addr));
			m_list.AddString(strMsg);

			break;
		}
		else if('3F' == nCmd)
		{
			// 回应对方打漏洞
			SOCKET*		pDessock				= (SOCKET *)&szBuffer[2];

			shutdown(sock, SD_BOTH);
			if(m_vctClient.end() == find(m_vctClient.begin(), m_vctClient.end(), *pDessock))
			{
				strMsg.Format("%s:%d error re-request.", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port));
				m_list.AddString(strMsg);
				break;
			}
			// 发送请求
			SOCKADDR_IN		sendaddr		= {0};

			sendaddr.sin_family = AF_INET;
			sendaddr.sin_port = addr.sin_port;
			sendaddr.sin_addr = addr.sin_addr;
			memset(szCmd, 0, sizeof(szCmd));
			strcpy(szCmd, "F4");
			memcpy(&szCmd[2], &sock, sizeof(sock));
			memcpy(&szCmd[6], &sendaddr, sizeof(sendaddr));
			send(*pDessock, szCmd, sizeof(sendaddr) + 6, 0);

			SOCKADDR_IN		ad1;
			int				nlen1		= sizeof(ad1);
			getpeername(*pDessock, (SOCKADDR *)&ad1, &nlen1);
			strMsg.Format("%s:%d re-request %s start hole.", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port), inet_ntoa(ad1.sin_addr));
			m_list.AddString(strMsg);

			break;
		}
		else
		{
			strMsg.Format("%s:%d error command.", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port));
			m_list.AddString(strMsg);
		}
	}

	shutdown(sock, SD_BOTH);
	strMsg.Format("%s:%d logout.", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port));
	m_list.AddString(strMsg);

	// 用户登录
	closesocket(sock);
	m_vctClient.erase(find(m_vctClient.begin(), m_vctClient.end(), sock));
}

void CNatServerDlg::OnListen()
{
	SOCKET			sock;
	SOCKADDR_IN		addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	bind(sock, (SOCKADDR *)&addr, sizeof(addr));

	listen(sock, 5);
	m_list.AddString("开始网络监听.");
	while(true)
	{
		SOCKET		client;
		SOCKADDR_IN	clientaddr;
		int			nlen			= sizeof(clientaddr);

		client = accept(sock, (SOCKADDR*)&clientaddr, &nlen);
		if(INVALID_SOCKET != client)
		{
			HANDLE		hThread;
			hThread = CreateThread(NULL, 0, OnThreadAccept, (LPVOID)client, 0, NULL);
			CloseHandle(hThread);
		}
	}
}