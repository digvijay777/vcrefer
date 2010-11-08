// ControlDrawDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "ControlDrawDlg.h"
#include ".\controldrawdlg.h"

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
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
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


// CControlDrawDlg 对话框



CControlDrawDlg::CControlDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDrawDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CControlDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TAB, m_tab);
	DDX_Control(pDX, IDC_BUTTON1, m_tranbtn);
	// 	DDX_Control(pDX, IDC_STATIC_g, m_groupbox);
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_LIST1, (CWnd &)m_ctlPlugList);
}

BEGIN_MESSAGE_MAP(CControlDrawDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_STATIC_TAB, OnTabCtrlSelectChange)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
END_MESSAGE_MAP()


// CControlDrawDlg 消息处理程序

BOOL CControlDrawDlg::OnInitDialog()
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
	m_tab.InitTabCtrl(&CSize(50, 25));
// 	m_tab.InsertItem(0, "hello");
// 	m_tab.InsertItem(1, "hi");
// 	m_tab.InsertItem(1, "Ha ha ha ha ha ha");
	m_tab.SetCurSel(0);

	m_radio1.LoadBitmap(IDB_RADIO);
	CBitmap		pBitmap;

	pBitmap.LoadBitmap(IDB_CHECK);
	m_ctlPlugList.SetImageList(IDB_CHECK, IDB_PROMPT);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CControlDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CControlDrawDlg::OnPaint() 
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
HCURSOR CControlDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CControlDrawDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CControlDrawDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CControlDrawDlg::OnTabCtrlSelectChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString str;
	str.Format("选择面板%d", m_tab.GetCurSel());
	SetDlgItemText(IDC_STATIC_TABSEL, str);
}
BOOL CControlDrawDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect	rect;

// 	GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// 
// 	pDC->FillSolidRect(&rect, RGB(0xff, 0xff, 0xff));
// 
// 	return TRUE;
// 
// 	return CDialog::OnEraseBkgnd(pDC);
/*
	static BITMAP bitmap = {0};
	if(NULL == m_bmpBk.m_hObject)
	{
		m_bmpBk.LoadBitmap(IDB_BITMAP_BK);
		if(NULL == m_bmpBk.m_hObject)
			return CDialog::OnEraseBkgnd(pDC);
		m_bmpBk.GetBitmap(&bitmap);
	}
	CDC		memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap*  pBmp = memDC.SelectObject(&m_bmpBk);
// 	CRect	rect;
	GetClientRect(&rect);
	//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
*/
	return CDialog::OnEraseBkgnd(pDC);
}

void CControlDrawDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CControlDrawDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
}

void CControlDrawDlg::OnBnClickedButton2()
{
	m_ctlPlugList.AddPlug("hi", "F:\\SVN\\common\\Object\\dsafdasfasdc\\fdaas\\fsdafa\\你好", "", "");
	m_ctlPlugList.AddPlug("hello", "type", "company", "path");
}

void CControlDrawDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_ctlPlugList.ClearPlug();
}

void CControlDrawDlg::OnBnClickedButton4()
{
	CString			strText;

	GetDlgItemText(IDC_BUTTON4, strText);
	if("全选" == strText)
	{
		m_ctlPlugList.CheckAll(TRUE);
		SetDlgItemText(IDC_BUTTON4, "全不选");
	}
	else
	{
		m_ctlPlugList.CheckAll(FALSE);
		SetDlgItemText(IDC_BUTTON4, "全选");
	}
}

void CControlDrawDlg::OnBnClickedButton5()
{
	m_ctlPlugList.AddPrompt("没有进行扫描扫描", "这是一行小字");
}
