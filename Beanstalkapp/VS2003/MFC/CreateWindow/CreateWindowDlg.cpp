// CreateWindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateWindow.h"
#include "CreateWindowDlg.h"
#include ".\createwindowdlg.h"

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


// CCreateWindowDlg 对话框



CCreateWindowDlg::CCreateWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateWindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateWindowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CCreateWindowDlg 消息处理程序

BOOL CCreateWindowDlg::OnInitDialog()
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

void CCreateWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCreateWindowDlg::OnPaint() 
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
HCURSOR CCreateWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateWindowDlg::OnBnClickedButton1()
{
	// 创建一个编辑框
	HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "", WS_CHILD | WS_VISIBLE | ES_NOHIDESEL, 
		10, 10, 100, 25, m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
	
}

TCHAR						g_szServiceName[]			= "MyFirstServiceTest";
HINSTANCE					g_hInst						= NULL;
HWND						g_hWnd						= NULL;


void CCreateWindowDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_hInst = AfxGetInstanceHandle();

	WNDCLASS	wndClass	= {0};

	wndClass.hInstance = g_hInst;
	wndClass.lpfnWndProc = ServerWndProc;
	wndClass.lpszClassName = g_szServiceName;

	int n = RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_szServiceName, "", WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, NULL, g_hInst, NULL);

}

LRESULT CALLBACK CCreateWindowDlg::ServerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_CLOSE:
		::DestroyWindow(g_hWnd);
		break;
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CCreateWindowDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
//	HWND hWnd = ::FindWindow(g_szServiceName, "");
	HWND hWnd = ::FindWindowEx(HWND_MESSAGE, NULL, g_szServiceName, "");
	if(NULL != hWnd)
		::SendMessage(hWnd, WM_CLOSE, 0, 0);
}
