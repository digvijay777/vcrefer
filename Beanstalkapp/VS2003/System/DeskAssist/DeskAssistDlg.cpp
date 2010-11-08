// DeskAssistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DeskAssist.h"
#include "DeskAssistDlg.h"
#include ".\deskassistdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeskAssistDlg 对话框



CDeskAssistDlg::CDeskAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeskAssistDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeskAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctlTab);
}

BEGIN_MESSAGE_MAP(CDeskAssistDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDeskAssistDlg 消息处理程序

BOOL CDeskAssistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ctlTab.InsertItem(0, "桌面一");
	m_ctlTab.InsertItem(1, "桌面二");
	m_ctlTab.InsertItem(2, "桌面三");

// 	m_ctlTab.ModifyStyle(0, TCS_VERTICAL);
	/*m_ctlTab.SendMessage(LB_SETHORIZONTALEXTENT, 1000);*/
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDeskAssistDlg::OnPaint() 
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
HCURSOR CDeskAssistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDeskAssistDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CDeskAssistDlg::OnBnClickedButton1()
{

}
