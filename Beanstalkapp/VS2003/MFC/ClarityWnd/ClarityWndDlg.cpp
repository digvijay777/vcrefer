// ClarityWndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClarityWnd.h"
#include "ClarityWndDlg.h"
#include ".\claritywnddlg.h"

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


// CClarityWndDlg 对话框



CClarityWndDlg::CClarityWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClarityWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hAlphaWnd = NULL;
}


void CClarityWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, m_btnSertch);
	DDX_Control(pDX, IDC_SLIDER1, m_Alpha);
}

BEGIN_MESSAGE_MAP(CClarityWndDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(UM_ENDSELECTWND, OnEndSelectWnd)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER1, OnNMThemeChangedSlider1)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CClarityWndDlg 消息处理程序

BOOL CClarityWndDlg::OnInitDialog()
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
	m_Alpha.SetRange(0, 255);
	m_Alpha.SetPos(100);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CClarityWndDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClarityWndDlg::OnPaint() 
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
HCURSOR CClarityWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClarityWndDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CString			str;
	int				iRes		= 0;

	GetDlgItemText(IDOK, str);
	if("取消透明" == str)
	{
		iRes = SetWindowLong(m_hAlphaWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd,GWL_EXSTYLE) & ~0x80000);
		if(0 == iRes)
		{
			str.Format("SetWindowLong Failed: %d", GetLastError());
			AfxMessageBox(str);
			return;
		}
		::SetWindowPos(m_hAlphaWnd, HWND_NOTOPMOST, -1, -1, -1, -1, SWP_NOSIZE);
		m_hAlphaWnd = NULL;
		SetDlgItemText(IDOK, "设置透明");
		return;
	}

	// 透明操作
	HWND			hWnd;

	m_hAlphaWnd = NULL;
	GetDlgItemText(IDC_EDIT1, str);
	sscanf(str.GetBuffer(), "0x%08X", (DWORD *)&(hWnd));

	if(!IsWindow(hWnd))
	{
		AfxMessageBox("不是有效窗体");
		return;
	}

	BOOL		bRes		= FALSE;

	iRes = SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE) | 0x80000);
	if(0 == iRes)
	{
		str.Format("SetWindowLong Failed: %d", GetLastError());
		AfxMessageBox(str);
		return;
	}

	bRes = ::SetLayeredWindowAttributes(hWnd, 0, (BYTE)m_Alpha.GetPos(), LWA_ALPHA);
	if(FALSE == bRes)
	{
		str.Format("SetLayeredWindowAttributes Failed: %d", GetLastError());
		AfxMessageBox(str);
		return;
	}
	//::SetWindowPos(hWnd, HWND_TOPMOST, -1, -1, -1, -1, SWP_NOSIZE);
	SetDlgItemText(IDOK, "取消透明");
	m_hAlphaWnd = hWnd;
}

/*
 *	完成选择
 */
LRESULT CClarityWndDlg::OnEndSelectWnd(WPARAM wParam, LPARAM lParam)
{
	CString		str;

	str.Format("0x%08X", wParam);
	SetDlgItemText(IDC_EDIT1, str.GetBuffer());
	return 0;
}

void CClarityWndDlg::OnNMThemeChangedSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;


}

void CClarityWndDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(NULL != m_hWnd)
	{
		::SetLayeredWindowAttributes(m_hAlphaWnd, 0, (BYTE)m_Alpha.GetPos(), LWA_ALPHA);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
