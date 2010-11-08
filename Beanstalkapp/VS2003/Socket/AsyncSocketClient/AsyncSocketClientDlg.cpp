// AsyncSocketClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AsyncSocketClient.h"
#include "AsyncSocketClientDlg.h"
#include ".\asyncsocketclientdlg.h"
#include <afxsock.h>
#include "MySocket.h"

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


// CAsyncSocketClientDlg 对话框



CAsyncSocketClientDlg::CAsyncSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsyncSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAsyncSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LT_MSG, m_msg);
}

BEGIN_MESSAGE_MAP(CAsyncSocketClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BT_CONNECT, OnBnClickedBtConnect)
	ON_BN_CLICKED(IDC_BT_CLOSE, OnBnClickedBtClose)
	ON_BN_CLICKED(IDC_BT_SEND, OnBnClickedBtSend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAsyncSocketClientDlg 消息处理程序

BOOL CAsyncSocketClientDlg::OnInitDialog()
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
	GetDlgItem(IDC_BT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(FALSE);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CAsyncSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAsyncSocketClientDlg::OnPaint() 
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
HCURSOR CAsyncSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAsyncSocketClientDlg::OnBnClickedButton1()
{
	SOCKET			sock;
	SOCKADDR_IN		addr;
	int				nRet;

	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.204");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4045);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == sock)
	{
		AfxMessageBox("创建套接字失败");
		return ;
	}
	do 
	{
		nRet = connect(sock, (SOCKADDR *)&addr, sizeof(SOCKADDR));
		if(SOCKET_ERROR == nRet)
		{
			AfxMessageBox("建立连接失败");
			break;
		}
		char	buff[100]	= {0};
		recv(sock, buff, 100, 0);
		AfxMessageBox(buff);
		strcpy(buff, "quitaaaaa");
		send(sock, buff, strlen(buff) + 1, 0);
		recv(sock, buff, 100, 0);
		AfxMessageBox(buff);
	} while (0);
	closesocket(sock);
}

void CAsyncSocketClientDlg::OnBnClickedBtConnect()
{
	//建立连接
	if(NULL != g_pSocket)
		return;
	g_pSocket = new CMySocket(m_hWnd);
	g_pSocket->Create();
	g_pSocket->Connect("192.168.1.204", 4045);
	//::SendDlgItemMessage(m_hWnd, IDC_BT_CLOSE, WM_ENABLE, (WPARAM)TRUE, 0);
	//SendDlgItemMessage(IDC_BT_CLOSE, WM_ENABLE, 1, 0);
	//::SendDlgItemMessage(m_hWnd, IDC_BT_SEND, WM_ENABLE, 0, 0);
	//::SendDlgItemMessage(m_hWnd, IDC_BT_CONNECT, WM_ENABLE, 1, 0);
	//GetDlgItem(IDC_BT_CLOSE)->EnableWindow(TRUE);
}

void CAsyncSocketClientDlg::OnBnClickedBtClose()
{
	// 关闭连接
	if(NULL == g_pSocket)
		return;
	g_pSocket->Close();
	delete g_pSocket;
	g_pSocket = NULL;
	GetDlgItem(IDC_BT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONNECT)->EnableWindow(TRUE);
}

void CAsyncSocketClientDlg::OnBnClickedBtSend()
{
	// 发送信息
	char	buff[4048]	= {0};
	int		length		= 0;

	GetDlgItemText(IDC_ET_SEND, buff, 4048);
	length = strlen(buff) + 1;
	if(1 == length)
	{
		AfxMessageBox("不能发送空字符串");
		return;
	}
	g_pSocket->Send(buff, length);
}

void CAsyncSocketClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(NULL != g_pSocket)
	{
		g_pSocket->Close();
		delete g_pSocket;
		g_pSocket = NULL;
	}
	CDialog::OnClose();
}
