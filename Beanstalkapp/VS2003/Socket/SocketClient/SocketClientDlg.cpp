// SocketClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"
#include ".\socketclientdlg.h"
#include <Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UM_RECV			(WM_USER+1)

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


// CSocketClientDlg 对话框



CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_MESSAGE(UM_RECV, OnUmRecv)
END_MESSAGE_MAP()


// CSocketClientDlg 消息处理程序

BOOL CSocketClientDlg::OnInitDialog()
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
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		MessageBox("初始化套接字失败");
		return TRUE;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		MessageBox("初始化套接字失败");
		return TRUE;
	}
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketClientDlg::OnPaint() 
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
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSocketClientDlg::OnBnClickedButton1()
{
	//同步连接服务器测试
	CString			str;
	SOCKET			server		= {0};
	SOCKADDR_IN		addr		= {0};
	int				nRet		= 0;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET != server)
	{
		do 
		{
			addr.sin_port = htons(8603);
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.204");
			//连接
			nRet = connect(server, (SOCKADDR*)&addr, sizeof(SOCKADDR));
			if(0 != nRet)
			{
				str.Format("connect error:%d\n", WSAGetLastError());
				break;
			}
			//接收数据
			char recvbuf[100];
			memset(recvbuf, 0, 100);
			nRet = recv(server, recvbuf, 100, 0);
			if(0 != nRet)
			{
				str.Format("recv error:%d\n", WSAGetLastError());
			}
			str.Format("Server say:%s\n", recvbuf);
		} while (0);
		closesocket(server);
	}
	else
	{
		str.Format("socket error:%d", WSAGetLastError());
	}

	MessageBox(str);
}

void CSocketClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	WSACleanup();
	CDialog::OnClose();
}

void CSocketClientDlg::OnBnClickedButton2()
{
	//异步连接服务器测试
	CString			str;
	SOCKET			server		= {0};
	SOCKADDR_IN		addr		= {0};
	int				nRet		= 0;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET != server)
	{
		do 
		{
			addr.sin_port = htons(4045);
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.204");
			//注册消息
			nRet = WSAAsyncSelect(server, m_hWnd, UM_RECV, FD_READ | FD_CLOSE | FD_CONNECT);
			if(SOCKET_ERROR == nRet)
			{
				str.Format("WSAAsyncSelect error:%d", WSAGetLastError());
			}
			//连接
			nRet = connect(server, (SOCKADDR*)&addr, sizeof(SOCKADDR));
		} while (0);
		//closesocket(server);
	}
	else
	{
		str.Format("socket error:%d", WSAGetLastError());
	}

	if(str.GetLength() > 0)
		MessageBox(str);
}

LRESULT CSocketClientDlg::OnUmRecv(WPARAM wParam, LPARAM lParam)
{
	if(WSAGETSELECTERROR(lParam))
	{
		closesocket(wParam);
		return 0;
	}

	char  szBuffer[100] = {0};
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		recv((SOCKET)wParam,  szBuffer, 100, 0);
		AfxMessageBox(szBuffer);
		break;
	case FD_CLOSE:
		closesocket((SOCKET)wParam);
		break;
	case FD_CONNECT:

		break;
	}
	return 0;
}
