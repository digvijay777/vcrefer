// DynamicResourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DynamicResource.h"
#include "DynamicResourceDlg.h"
#include ".\dynamicresourcedlg.h"
#include "../../../ExtClass/xres.h"

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


// CDynamicResourceDlg 对话框



CDynamicResourceDlg::CDynamicResourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDynamicResourceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDynamicResourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDynamicResourceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CDynamicResourceDlg 消息处理程序

BOOL CDynamicResourceDlg::OnInitDialog()
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
// 	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE) | 0x80000);
// 	::SetLayeredWindowAttributes(GetSafeHwnd(), 0, 150, LWA_ALPHA);
// 	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDynamicResourceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDynamicResourceDlg::OnPaint() 
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
HCURSOR CDynamicResourceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDynamicResourceDlg::OnBnClickedButton1()
{
	int		nRes		= 0;

	nRes = XRes::AloneMsgbox(GetSafeHwnd(), "hello", "hi", MB_YESNO|MB_ICONQUESTION);
	if(IDYES == nRes)
	{
		TRACE("select [yes]\n");
	}
	else
	{
		TRACE("select [no]\n");
	}
}

void CDynamicResourceDlg::OnBnClickedButton2()
{
// 	GetDesktopWindow()->EnableWindow(FALSE);
// 	Sleep(5000);
// 	GetDesktopWindow()->EnableWindow(TRUE);
	// MessageBox("hello", NULL, MB_OK/*|MB_DEFAULT_DESKTOP_ONLY|MB_SYSTEMMODAL*/);
	int			nRes		= 0;

	nRes = XRes::TimeoutMsgbox(GetSafeHwnd(), L"hello", L"hi", MB_ICONQUESTION | MB_YESNO, 1000);
	if(IDYES == nRes)
	{
		TRACE("select [yes]\n");
	}
	else
	{
		TRACE("select [no]\n");
	}
}
