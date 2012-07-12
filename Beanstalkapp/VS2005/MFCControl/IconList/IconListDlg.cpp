// IconListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IconList.h"
#include "IconListDlg.h"

#define UM_NOTIFY			WM_USER + 1

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


// CIconListDlg 对话框




CIconListDlg::CIconListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIconListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIconListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_ck);
}

BEGIN_MESSAGE_MAP(CIconListDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_NOTIFY, &CIconListDlg::OnUmNotify)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, &CIconListDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CIconListDlg 消息处理程序

BOOL CIconListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitGdiplus();
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
	m_ilc.m_imageRadio = ImageFromResouce(AfxGetInstanceHandle(), _T("PNG"),
		MAKEINTRESOURCE(IDR_PNG1));
	m_ilc.m_imageItemBk = ImageFromResouce(AfxGetInstanceHandle(), _T("PNG"),
		MAKEINTRESOURCE(IDR_PNG2));
	m_ilc.m_imageItemEt = ImageFromResouce(AfxGetInstanceHandle(), _T("PNG"),
		MAKEINTRESOURCE(IDR_PNG3));
	m_ilc.m_uNotifyMsg = UM_NOTIFY;
	m_ilc.m_hNotifyWnd = GetSafeHwnd();
	m_ilc.SubclassWindow(::GetDlgItem(GetSafeHwnd(), IDC_ICONLIST));
	m_ilc.SetGroupCount(2);
	// 添加子项
	m_ilc.AddItem(0, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1)),
		L"电脑清理", 0);
	m_ilc.AddItem(0, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON2)),
		L"优化加速", 1);
	m_ilc.AddItem(0, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON3)),
		L"漏洞修复", 2);
	m_ilc.AddItem(1, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON4)),
		L"功能大全", 3);
	m_ilc.AddItem(1, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON5)),
		L"软件管家", 4);
	m_ilc.UpdateGroup(0);
	m_ilc.UpdateGroup(1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIconListDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIconListDlg::OnPaint()
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
HCURSOR CIconListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
 *	接收控件消息
 */
LRESULT CIconListDlg::OnUmNotify(WPARAM wParam, LPARAM lParam)
{
	LPTSTR		szText[5]	= {
		_T("电脑清理"),
		_T("优化加速"),
		_T("漏洞修复"),
		_T("功能大全"),
		_T("软件管家")
	};
	
	if(wParam > 5)
	{
		return 0;
	}

	CString			str;

	str.Format(_T("单击了%s模式下: %s"),
		(0x1 & lParam)?_T("编辑模式"):_T("正常模式"),
		szText[wParam]);
	MessageBox(str);
	return 0;
}

void CIconListDlg::OnBnClickedCheck1()
{
	m_ilc.SetEditMode(BST_CHECKED == m_ck.GetCheck());
}
