// ProxySocksDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProxySocks.h"
#include "ProxySocksDlg.h"
#include ".\proxysocksdlg.h"
#include <afxsock.h>
#include "../../../ExtClass/Net/XSocksProxy.h"
#include "../../../ExtClass/XNet.h"

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


// CProxySocksDlg 对话框



CProxySocksDlg::CProxySocksDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProxySocksDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProxySocksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProxySocksDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CProxySocksDlg 消息处理程序

BOOL CProxySocksDlg::OnInitDialog()
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
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CProxySocksDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProxySocksDlg::OnPaint() 
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
HCURSOR CProxySocksDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProxySocksDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CXSocksProxy		proxy;
	CHAR				szBuff[1024];

	if(FALSE == proxy.Connect("192.168.1.6", 1080, "xxj", "xxx"))
		return;

// 	if(FALSE == proxy.ConnectSocks5(NULL, NULL, "161.250.41.200", 8603))
// 		return;
// 	if(FALSE == proxy.ProxySocks5("161.250.41.200", 8603))
// 		return;
	if(FALSE == proxy.ProxySocks5("161.250.41.200", 8603))
		return ;

	memset(szBuff, 0, 1024);
	recv(proxy.m_hSocket, szBuff, 1024, 0);
	AfxMessageBox(szBuff);
}

void CProxySocksDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CSocket		sock;
	CHAR		szBuff[1024];

	if(FALSE == sock.Create())
		return;

	if(FALSE == sock.Connect("161.250.41.200", 8603))
		return;

	memset(szBuff, 0, 1024);
	if(FALSE == sock.Receive(szBuff, 1024))
		return;
	
	AfxMessageBox(szBuff);
}

void CProxySocksDlg::OnBnClickedButton3()
{
	SOCKET			sock				= INVALID_SOCKET;
	LPCSTR			lpGet				= "GET /\r\n";
	CHAR			szBuffer[1024]		= {0};

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(FALSE == XNet::ConnectWidthProxy4(sock, "192.168.1.16", 80, "yj.net", 1080))
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(FALSE == XNet::ConnectWidthProxy5(sock, "192.168.1.16", 80, "yj.net", 1080, "yj", "1234"))
		{
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(FALSE == XNet::ConnectWidthProxy5(sock, "192.168.1.16", 80, "yj.net", 1080, "yj", "123"))
				return;
		}
	}
	
	send(sock, lpGet, strlen(lpGet) + 1, 0);
	recv(sock, szBuffer, sizeof(szBuffer), 0);

}
