// IEPasswdWatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IEPasswdWatch.h"
#include "IEPasswdWatchDlg.h"
#include ".\iepasswdwatchdlg.h"
#include "ExternIEOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIEPasswdWatchDlg 对话框


CIEPasswdWatchDlg::CIEPasswdWatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIEPasswdWatchDlg::IDD, pParent)
	,m_bCapture(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIEPasswdWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIEPasswdWatchDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CIEPasswdWatchDlg 消息处理程序

BOOL CIEPasswdWatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIEPasswdWatchDlg::OnPaint() 
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
HCURSOR CIEPasswdWatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIEPasswdWatchDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
// 	OnOK();
}

void CIEPasswdWatchDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

void CIEPasswdWatchDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bCapture)
	{
		ReleaseCapture();
		m_bCapture = FALSE;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

		TCHAR			buf[100]	= {0};
		POINT			pt;
		HWND			hWnd;

		GetCursorPos(&pt);
		hWnd = ::WindowFromPoint(pt);
		if(IsWindow(hWnd))
		{
			::GetClassName(hWnd, (LPTSTR)&buf, 100);
			if(_tcsicmp(buf, _T("Internet Explorer_Server")) == 0)
			{
				::ScreenToClient(hWnd, &pt);
				ExtIE::CoInitialize();
				AfxMessageBox(ExtIE::GetPassWord(ExtIE::GetDocInterface(hWnd), pt));
				ExtIE::CoUninitialize();
			}
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

//BOOL CIEPasswdWatchDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: Add your specialized code here and/or call the base cla)
//	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
//}

BOOL CIEPasswdWatchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->hwnd == GetDlgItem(IDC_BUTTON1)->m_hWnd)
	{
		if(WM_LBUTTONDOWN == pMsg->message)
		{
			SetCapture();
			m_bCapture = TRUE;
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
