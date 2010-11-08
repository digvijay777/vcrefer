// ShutdownTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShutdownTest.h"
#include "ShutdownTestDlg.h"
#include ".\shutdowntestdlg.h"

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


// CShutdownTestDlg 对话框



CShutdownTestDlg::CShutdownTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShutdownTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShutdownTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShutdownTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CShutdownTestDlg 消息处理程序

BOOL CShutdownTestDlg::OnInitDialog()
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

void CShutdownTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CShutdownTestDlg::OnPaint() 
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
HCURSOR CShutdownTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShutdownTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

void CShutdownTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//获取关机权限
	GetPrivilege();
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);

	// 另一种方法
// 	HANDLE				hProcess	= NULL;
// 	HANDLE				hToken		= NULL;
// 	TOKEN_PRIVILEGES	Privileges	= {0};
// 	LUID				luid		= {0};
// 
// 	hProcess = GetCurrentProcess();
// 	// 下面为打开当前进程对话
// 	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
// 	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
// 
// 	Privileges.PrivilegeCount			= 1;
// 	Privileges.Privileges[0].Luid		= luid; 
// 	Privileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 
// 
// 	AdjustTokenPrivileges(hToken, FALSE, &Privileges, NULL, NULL, NULL);
// 	// 重启
// 	if(FALSE == InitiateSystemShutdownEx(NULL, "系统维护", 0, FALSE, TRUE, 
// 		SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_SECURITY | SHTDN_REASON_FLAG_PLANNED))
// 	{
// 		NLog::FolderFileLog(DBG_LOGDIR, DBG_LOGFILE, "Reboot InitiateSystemShutdownEx error:%d", GetLastError());
// 	}
}



void CShutdownTestDlg::GetPrivilege(void)
{
	HANDLE				hProcess	= NULL;
	HANDLE				hToken		= NULL;
	TOKEN_PRIVILEGES	Privileges	= {0};
	LUID				luid		= {0};
	
	hProcess = GetCurrentProcess();
	//下面为打开当前进程对话
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);

	Privileges.PrivilegeCount			= 1;
	Privileges.Privileges[0].Luid		= luid; 
	Privileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 

	AdjustTokenPrivileges(hToken, FALSE, &Privileges, NULL, NULL, NULL);
}

void CShutdownTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	GetPrivilege();
	ExitWindowsEx(EWX_REBOOT, 0);
}

void CShutdownTestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	GetPrivilege();
	ExitWindowsEx(EWX_LOGOFF, 0);

	//InitiateSystemShutdownEx();未登录用户
}

void CShutdownTestDlg::RebootSystem()
{
	HANDLE				hProcess	= NULL;
	HANDLE				hToken		= NULL;
	TOKEN_PRIVILEGES	Privileges	= {0};
	LUID				luid		= {0};

	hProcess = GetCurrentProcess();
	// 下面为打开当前进程对话
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);

	Privileges.PrivilegeCount			= 1;
	Privileges.Privileges[0].Luid		= luid; 
	Privileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 

	AdjustTokenPrivileges(hToken, FALSE, &Privileges, NULL, NULL, NULL);
	// 重启
	if(FALSE == InitiateSystemShutdownEx(NULL, "系统维护", 0, FALSE, TRUE, 
		SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_SECURITY | SHTDN_REASON_FLAG_PLANNED))
	{
		TRACE("Reboot InitiateSystemShutdownEx error:%d", GetLastError());
	}
}