// SendMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SendMsg.h"
#include "SendMsgDlg.h"
#include ".\sendmsgdlg.h"

#include "ProcessWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendMsgDlg 对话框



CSendMsgDlg::CSendMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMsgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSendMsgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_WND_FIND, OnBnClickedWndFind)
	ON_BN_CLICKED(IDC_WND_SEND, OnBnClickedWndSend)
	ON_BN_CLICKED(IDC_BT_LIST, OnBnClickedBtList)
END_MESSAGE_MAP()


// CSendMsgDlg 消息处理程序

BOOL CSendMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

// 	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	CenterWindow();
	
	EnableSendWndGrups(false);
	SetDlgItemText(IDC_RT_MSG, _T(""));

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   

	((CButton*)GetDlgItem(IDC_WND_HANDLE_HEX))->SetCheck(1);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSendMsgDlg::OnPaint() 
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
HCURSOR CSendMsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendMsgDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
// 	OnOK();
}

void CSendMsgDlg::EnableSendWndGrups(BOOL babled)
{
	GetDlgItem(IDC_WND_MSG)->EnableWindow(babled);
	GetDlgItem(IDC_WND_SEND)->EnableWindow(babled);
	GetDlgItem(IDC_RT_HANDLE_HEX)->EnableWindow(babled);
	GetDlgItem(IDC_RT_WPARAM)->EnableWindow(babled);
	GetDlgItem(IDC_RT_LPARAM)->EnableWindow(babled);
	if(FALSE == babled)
	{
		SetDlgItemText(IDC_RT_TITLE, _T(""));
		SetDlgItemText(IDC_RT_CLASS, _T(""));
		SetDlgItemText(IDC_RT_STATUS, _T(""));
		SetDlgItemText(IDC_RT_HANDLE, _T(""));
	}
}

void CSendMsgDlg::OnBnClickedWndFind()
{
	// TODO: Add your control notification handler code here
	TCHAR			szText[50]		= {0};
	HWND			hWnd			= NULL;
	EnableSendWndGrups(false);
	SetDlgItemText(IDC_RT_MSG, _T(""));
	do 
	{
		//句柄优先
		GetDlgItemText(IDC_WND_HANDLE, szText, 49);
		if(_tcslen(szText) > 0)
		{
			if(((CButton*)GetDlgItem(IDC_WND_HANDLE_HEX))->GetCheck())
			{
				//十六进制
				hWnd = (HWND)_tcstol(szText, NULL, 16);
			}
			else
			{
				//十进制
				hWnd = (HWND)_tcstol(szText, NULL, 10);
			}
			break;
		}
		//窗口类名优先
		GetDlgItemText(IDC_WND_CLASS, szText, 49);
		if(_tcslen(szText) > 0)
		{
			hWnd = ::FindWindow(szText, NULL);
			break;
		}
		//窗口类名最后
		GetDlgItemText(IDC_WND_TITLE, szText, 49);
		if(_tcslen(szText) > 0)
		{
			hWnd = ::FindWindow(NULL, szText);
			break;
		}
	} while (false);

	if(NULL == hWnd || !::IsWindow(hWnd))
	{
		SetDlgItemText(IDC_RT_MSG, _T("找不到指定窗口"));
		return;
	}

	CWnd* pWnd = CWnd::FromHandle(hWnd);
	if(NULL == pWnd)
	{
		SetDlgItemText(IDC_RT_MSG, _T("指定窗口拒绝操作"));
		return;
	}
	//操作窗体
	pWnd->GetWindowText(szText, 49);
	SetDlgItemText(IDC_RT_TITLE, szText);
	WINDOWPLACEMENT	wt = {0};
	wt.length = sizeof(WINDOWPLACEMENT);
	if(pWnd->GetWindowPlacement(&wt))
	{
		switch(wt.flags)
		{
		case SW_SHOW:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOW"));
			break;
		case SW_HIDE:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_HIDE"));
			break;
// 		case SW_MAXIMIZE:
// 			SetDlgItemText(IDC_RT_STATUS, _T("SW_MAXIMIZE"));
// 			break;
// 		case SW_MINIMIZE:
// 			SetDlgItemText(IDC_RT_STATUS, _T("SW_MINIMIZE"));
// 			break;
		case SW_RESTORE:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_RESTORE"));
			break;
		case SW_SHOWMAXIMIZED:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOWMAXIMIZED"));
			break;
		case SW_SHOWMINIMIZED:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOWMINIMIZED"));
			break;
		case SW_SHOWMINNOACTIVE:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOWMINNOACTIVE"));
			break;
		case SW_SHOWNA:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOWNA"));
			break;
		case SW_SHOWNOACTIVATE:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOWNOACTIVATE"));
			break;
		case SW_SHOWNORMAL:
			SetDlgItemText(IDC_RT_STATUS, _T("SW_SHOWNORMAL"));
			break;
		default:
			SetDlgItemText(IDC_RT_STATUS, _T("读取失败"));
			break;
		}
	}
	else
		SetDlgItemText(IDC_RT_STATUS, _T("读取失败"));
	GetClassName(hWnd, szText, 49);
	SetDlgItemText(IDC_RT_CLASS, szText);
	_stprintf(szText, _T("0x%08x"), hWnd);
	SetDlgItemText(IDC_RT_HANDLE, szText);
	EnableSendWndGrups(true);
}

void CSendMsgDlg::OnBnClickedWndSend()
{
	// TODO: Add your control notification handler code here
	HWND		hWnd		= NULL;
	int			msg			= 0;
	TCHAR		szText[50]	= {0};
	WPARAM		wParam		= 0;
	LPARAM		lParam		= 0;

	GetDlgItemText(IDC_WND_MSG, szText, 49);
	if(((CButton*)GetDlgItem(IDC_RT_HANDLE_HEX))->GetCheck())
	{
		//十六进制
		msg = _tcstol(szText, NULL, 16);
	}
	else
	{
		//十进制
		msg = _tcstol(szText, NULL, 10);
	}
	GetDlgItemText(IDC_RT_HANDLE, szText, 49);
	hWnd = (HWND)_tcstol(szText, NULL, 16);
	if(!IsWindow(hWnd))
	{
		EnableSendWndGrups(false);
		SetDlgItemText(IDC_RT_MSG, _T("窗口句柄无效，可能因指定窗口被关闭"));
		return;
	}
	GetDlgItemText(IDC_RT_WPARAM, szText, 49);
	wParam = (WPARAM)_tcstol(szText, NULL, 10);
	GetDlgItemText(IDC_RT_LPARAM, szText, 49);
	lParam = (WPARAM)_tcstol(szText, NULL, 10);
	::PostMessage(hWnd, msg, wParam, lParam);
}

void CSendMsgDlg::OnBnClickedBtList()
{
	// 进程列表
	CProcessWndDlg			dlg;

	if(IDOK == dlg.DoModal())
	{
		((CButton*)GetDlgItem(IDC_WND_HANDLE_HEX))->SetCheck(1);
		CString		str;
		str.Format("%08x", dlg.m_hSelWnd);
		SetDlgItemText(IDC_WND_HANDLE, str.GetBuffer());
		OnBnClickedWndFind();
	}
}
