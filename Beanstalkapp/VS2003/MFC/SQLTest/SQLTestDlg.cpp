// SQLTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SQLTest.h"
#include "SQLTestDlg.h"
#include ".\sqltestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF")
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


// CSQLTestDlg 对话框



CSQLTestDlg::CSQLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSQLTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSQLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSQLTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CSQLTestDlg 消息处理程序

BOOL CSQLTestDlg::OnInitDialog()
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

void CSQLTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSQLTestDlg::OnPaint() 
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
HCURSOR CSQLTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 备份数据
void CSQLTestDlg::OnBnClickedButton1()
{
	_ConnectionPtr	m_spConnect;
	LPSTR			lpCon = "Provider=SQLOLEDB.1;User ID=sa;Password=123123;"
							"Persist Security Info=True;Initial Catalog=master;Network Address=192.168.1.22";
// 	LPSTR			lpSqlBk = "backup database small to disk='c:\\small2.dat' with init";
	LPSTR			lpSqlBk = "backup database small to disk='small4.dat' with init";
	LPSTR			lpSqlRe = "ALTER DATABASE small SET OFFLINE WITH ROLLBACK IMMEDIATE;"
							"restore database small from disk='c:\\small2.dat' with replace;"
							"ALTER DATABASE small SET ONLINE WITH ROLLBACK IMMEDIATE";
	CoInitialize(NULL);
	//连接数据库
	try{
		m_spConnect.CreateInstance(__uuidof(Connection));
		//m_spConnect->put_ConnectionTimeout(3000);
		m_spConnect->ConnectionTimeout = 3;
		m_spConnect->Open(lpCon, "", "", -1);

		AfxMessageBox("连接成功");
		m_spConnect->Execute(lpSqlBk, NULL, 0);
		AfxMessageBox("备份成功");
		// 连接成功
		m_spConnect->Close();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description(), MB_OK | MB_ICONERROR);
		return ;
	}
	CoUninitialize();
}
