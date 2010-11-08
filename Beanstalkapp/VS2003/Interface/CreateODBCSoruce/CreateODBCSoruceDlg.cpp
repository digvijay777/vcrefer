// CreateODBCSoruceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateODBCSoruce.h"
#include "CreateODBCSoruceDlg.h"
#include ".\createodbcsorucedlg.h"

#include "odbcinst.h"
#pragma comment(lib, "odbccp32.lib")

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


// CCreateODBCSoruceDlg 对话框



CCreateODBCSoruceDlg::CCreateODBCSoruceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateODBCSoruceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hBrush = CreateSolidBrush(RGB(0xff, 0xff, 0xff));
}

CCreateODBCSoruceDlg::~CCreateODBCSoruceDlg()
{
	if(NULL != m_hBrush)
		DeleteObject(m_hBrush);
}

void CCreateODBCSoruceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateODBCSoruceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCreateODBCSoruceDlg 消息处理程序

BOOL CCreateODBCSoruceDlg::OnInitDialog()
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

void CCreateODBCSoruceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCreateODBCSoruceDlg::OnPaint() 
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
HCURSOR CCreateODBCSoruceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateODBCSoruceDlg::OnBnClickedOk()
{
	char	szName[MAX_PATH]		= {0};
	char	szPath[MAX_PATH]		= {0};
	char	szFullPath[MAX_PATH]	= {0};

	GetDlgItemText(IDC_EDIT1, szPath, MAX_PATH);
	GetDlgItemText(IDC_EDIT2, szName, MAX_PATH);

	if(strlen(szName) == 0 || strlen(szPath) == 0)
	{
		AfxMessageBox("请填写相关信息");
		return;
	}

	strcpy(szFullPath, szPath);
	char*	pChar = strrchr(szPath, '\\');
	if(NULL != pChar)
		*pChar = 0;
	//创建ODBC
	char	szDNS[1024]		= {0};
	sprintf(szDNS, "DSN=%s;DBQ=%s;DEFAULTDIR=%s;;", szName, szFullPath, szPath);
	for(int i = strlen(szDNS) - 1; i >=0; i--)
	{
		if(szDNS[i] == ';')
			szDNS[i] = 0;
	}
	bool bRet = SQLConfigDataSource(NULL, 
		ODBC_ADD_SYS_DSN, 
		"Microsoft Access Driver (*.mdb)\0",
		szDNS);
	AfxMessageBox(bRet?"添加成功":"添加失败");
}

HBRUSH CCreateODBCSoruceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(GetDlgItem(IDC_EDIT1)->m_hWnd == pWnd->m_hWnd)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_hBrush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CCreateODBCSoruceDlg::OnBnClickedButton1()
{
	CFileDialog fd(true, "*.mdb", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "数据库文件(*.mdb)|*.mdb|所有文件(*.*)|*.*||", this);
	if(fd.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT1, fd.GetPathName());
	}
}
