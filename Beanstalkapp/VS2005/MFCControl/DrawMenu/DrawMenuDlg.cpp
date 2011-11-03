// DrawMenuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawMenu.h"
#include "DrawMenuDlg.h"
#include "SkinMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawMenuDlg 对话框




CDrawMenuDlg::CDrawMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawMenuDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawMenuDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDrawMenuDlg::OnBnClickedButton1)
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()


// CDrawMenuDlg 消息处理程序

BOOL CDrawMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrawMenuDlg::OnPaint()
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
HCURSOR CDrawMenuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDrawMenuDlg::OnBnClickedButton1()
{
	CRect		rect;
	CMenu*		pSysMenu		= GetSystemMenu(FALSE);

	GetDlgItem(IDC_BUTTON1)->GetWindowRect(&rect);
	CSkinMenu		menu;

	menu.CreatePopupMenu();
	menu.LoadImages(IDB_MENU);
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 100, _T("测试1"));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 1, _T("测试2"));
	menu.AppendMenu(MF_SEPARATOR|MF_OWNERDRAW, 0, _T(""));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 2, _T("测试3"));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 3, _T("测试4"));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 4, _T("测试5"));
	//menu.Attach(pSysMenu->GetSafeHmenu());
	menu.TrackPopupMenu(TPM_NOANIMATION, rect.left, rect.bottom, this);
	//menu.Detach();
}

void CDrawMenuDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
}
