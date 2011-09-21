// NoManifestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NoManifest.h"
#include "NoManifestDlg.h"
#include <shlobj.h>

#include "UACAutoRun.h"

#pragma comment(lib, "shell32.lib")

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


// CNoManifestDlg 对话框




CNoManifestDlg::CNoManifestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoManifestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNoManifestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNoManifestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CNoManifestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CNoManifestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNoManifestDlg 消息处理程序

BOOL CNoManifestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNoManifestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNoManifestDlg::OnPaint()
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
//
HCURSOR CNoManifestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNoManifestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
	if(IsUserAnAdmin())
	{
		MessageBox(_T("是管理员权限"));
	}
	else
	{
		MessageBox(_T("不是管理员权限"));
	}
}

void CNoManifestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	HANDLE hToken = NULL; 
	TOKEN_PRIVILEGES tkp; 
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS,   &hToken)   ) 
	{ 
		if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid)   ) 
		{ 
			if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0x10, (PTOKEN_PRIVILEGES)NULL, 0))
				MessageBox(_T("Errored step:3"));
		} 
		else MessageBox(_T("Errored step:2"));
	} 
	else MessageBox(_T("Errored step:1"));
	if(hToken)
		CloseHandle(hToken); 
	//
// 	HANDLE		hToken		= NULL;
	HANDLE		hUser		= NULL;
	HANDLE		hUserToken	= NULL;
	BOOL		bRest		= FALSE;

	GetTokenByProcName(hToken, _T("winlogon.exe"));
	OpenProcessToken(hToken, MAXIMUM_ALLOWED, &hUser);

// 	DuplicateTokenEx(hUser, MAXIMUM_ALLOWED, NULL
// 		, SecurityIdentification, TokenPrimary, &hUserToken);

	bRest = ImpersonateLoggedOnUser(hUser);
	RunAsUAC();
	RevertToSelf();
}
