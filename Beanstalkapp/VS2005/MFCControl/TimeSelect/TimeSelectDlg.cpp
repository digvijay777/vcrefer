// TimeSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimeSelect.h"
#include "TimeSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimeSelectDlg 对话框




CTimeSelectDlg::CTimeSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSelectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_2, m_ctlRichTest);
}

BEGIN_MESSAGE_MAP(CTimeSelectDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTimeSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTimeSelectDlg 消息处理程序

BOOL CTimeSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	BYTE		szWeekFlag[21]		= {0};
	BYTE		szWeekFlagEx[84]	= {0};

	szWeekFlag[1] = 0xff;
	szWeekFlag[4] = 0xff;
	szWeekFlag[7] = 0xff;
	szWeekFlag[10] = 0xff;
	szWeekFlag[13] = 0xff;
	szWeekFlag[16] = 0xff;
	szWeekFlag[19] = 0xff;
	m_timesel.SetWeekFlag(szWeekFlag, 21);
	szWeekFlagEx[12 * 0 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 1 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 2 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 3 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 4 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 5 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 6 + 8 / 2] = 0xa;
	m_ctlTime.SetWeekFlagEx(szWeekFlagEx, 84);
	m_timesel.SubclassWindow(::GetDlgItem(GetSafeHwnd(), IDC_STATIC_1));
	m_ctlTime.SubclassWindow(::GetDlgItem(GetSafeHwnd(), IDC_STATIC_3));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTimeSelectDlg::OnPaint()
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
HCURSOR CTimeSelectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTimeSelectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	static	int		i = 0;

	m_timesel.EnableWindow(i);
	i = (i + 1) % 2;
}
