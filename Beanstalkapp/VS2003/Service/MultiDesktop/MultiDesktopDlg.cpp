// MultiDesktopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiDesktop.h"
#include "MultiDesktopDlg.h"
#include ".\multidesktopdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiDesktopDlg 对话框
#pragma comment(lib, "Rpcrt4.lib")


CMultiDesktopDlg::CMultiDesktopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiDesktopDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CMultiDesktopDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_NEWDESKTOP, OnBnClickedBtNewdesktop)
	ON_BN_CLICKED(IDC_BT_SWITCHDESKTOP, OnBnClickedBtSwitchdesktop)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CMultiDesktopDlg 消息处理程序

BOOL CMultiDesktopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultiDesktopDlg::OnPaint() 
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
HCURSOR CMultiDesktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultiDesktopDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	RefurbishDesktop();
// 	OnOK();
}
//////////////////////////////////////////////////////////////////////////
void CMultiDesktopDlg::RefurbishDesktop()
{
	while(m_list.GetCount() > 0)
	{
		m_list.DeleteString(0);
	}

// 	STARTUPINFO		si		= {0};
// 	::GetStartupInfo(&si);
// 	m_list.AddString(si.lpDesktop);
	EnumWindowStations(EnumWindowStationProc, (LPARAM)m_list.GetSafeHwnd());
}
struct	ENUMINFO
{
	HWND		hList;
	TCHAR		szStation[260];
};
BOOL CALLBACK CMultiDesktopDlg::EnumDesktopProc(LPTSTR lpszDesktop, LPARAM lParam)
{
	ENUMINFO*	pInfo	= (ENUMINFO *)lParam;
	CListBox*	pList	 = (CListBox *)CWnd::FromHandle(pInfo->hList);
	TCHAR		szDesk[MAX_PATH] ={0};

	_stprintf(szDesk, "%s\\%s", pInfo->szStation, lpszDesktop);
	pList->AddString(szDesk);
	return TRUE;
}
BOOL CALLBACK CMultiDesktopDlg::EnumWindowStationProc(LPTSTR lpszWindowStation, LPARAM lParam)
{
	HWINSTA  hwSta = OpenWindowStation(lpszWindowStation, FALSE, WINSTA_ALL_ACCESS);
	if(NULL != hwSta)
	{
		ENUMINFO	info;
		info.hList = (HWND)lParam;
		_tcscpy(info.szStation, lpszWindowStation);
		EnumDesktops(hwSta, EnumDesktopProc, (LPARAM)&info);
		CloseWindowStation(hwSta);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
void CMultiDesktopDlg::OnBnClickedBtNewdesktop()
{
	// 创建新桌面
	TCHAR		szDesktop[MAX_PATH]	= {0};
	TCHAR		szErr[MAX_PATH]		= {0};

	GetDlgItemText(IDC_ET_NEWDESKTOP, szDesktop, MAX_PATH);
	if(_tcslen(szDesktop) < 1)
	{
		AfxMessageBox("请输入新桌面名");
		return;
	}
	
	HDESK		hDesk;
	DWORD		dwAccess = DESKTOP_ENUMERATE | DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP;

	hDesk = ::CreateDesktop(szDesktop, NULL, NULL, 0, dwAccess, NULL);
	if(NULL == hDesk)
	{
		_stprintf(szErr, _T("创建桌面失败:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
	// 创建explorer进程
	STARTUPINFO			si		= {0};
	PROCESS_INFORMATION	pi		= {0};

	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = szDesktop;

	if(FALSE != CreateProcess(NULL, "C:\\windows\\explorer.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	//CloseDesktop(hDesk);
	RefurbishDesktop();
}
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CMultiDesktopDlg::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if(NULL == ::GetParent(hwnd))
	{
		*((HWND *)lParam) = hwnd;
		return FALSE;
	}
	return TRUE;
}
void CMultiDesktopDlg::OnBnClickedBtSwitchdesktop()
{
	// 切换桌面
	TCHAR		szDesktop[MAX_PATH]	= {0};
	TCHAR		szErr[MAX_PATH]		= {0};
	TCHAR*		pUnderline			= NULL;
	
	m_list.GetText(m_list.GetCurSel(), szDesktop);
	if(0 == szDesktop[0])
	{
		AfxMessageBox("请选择桌面");
		return;
	}

	HDESK		hDesk;
	DWORD		dwAccess = DESKTOP_ENUMERATE | DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP;
	
	pUnderline = _tcsrchr(szDesktop, '\\');
	if(NULL != pUnderline)
		pUnderline += 1;
	else
		pUnderline = szDesktop;
	hDesk = OpenDesktop(pUnderline, 0, FALSE, /*dwAccess*/MAXIMUM_ALLOWED);
	if(NULL == hDesk)
	{
		_stprintf(szErr, _T("打开桌面失败:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	// 枚举桌面窗体
// 	HWND		hExplorer	= NULL;
// 
// 	EnumDesktopWindows(hDesk, EnumWindowsProc, (LPARAM)&hExplorer);
// 	//
// 	CloseDesktop(hDesk);
// 	return;
	RpcRevertToSelf(); 
//	ShowWindow(SW_HIDE);
//	if(FALSE != ::SetThreadDesktop(hDesk))
	{
		//::SetParent(m_hWnd, hExplorer);
// 		::SetParent(m_hWnd, hExplorer);
		::SwitchDesktop(hDesk);
//		ShowWindow(SW_SHOW);
	}
// 	else
// 	{
// 		_stprintf(szErr, _T("设置线程桌面失败:%d"), GetLastError());
// 		AfxMessageBox(szErr);
// 		return;
// 	}
}

void CMultiDesktopDlg::OnBnClickedButton1()
{
	TCHAR		szDesktop[MAX_PATH]	= {0};
	TCHAR		szErr[MAX_PATH]		= {0};
	TCHAR*		pUnderline			= NULL;

	m_list.GetText(m_list.GetCurSel(), szDesktop);
	if(0 == szDesktop[0])
	{
		AfxMessageBox("请选择桌面");
		return;
	}

	HDESK		hDesk;
	DWORD		dwAccess = DESKTOP_ENUMERATE | DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP;

	pUnderline = _tcsrchr(szDesktop, '\\');
	if(NULL != pUnderline)
		pUnderline += 1;
	else
		pUnderline = szDesktop;
	hDesk = OpenDesktop(pUnderline, 0, FALSE, /*dwAccess*/MAXIMUM_ALLOWED);
	if(NULL == hDesk)
	{
		_stprintf(szErr, _T("打开桌面失败:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	// 枚举桌面窗体
	// 	HWND		hExplorer	= NULL;
	// 
	// 	EnumDesktopWindows(hDesk, EnumWindowsProc, (LPARAM)&hExplorer);
	// 	//
	// 	CloseDesktop(hDesk);
	// 	return;
	if(FALSE == CloseDesktop(hDesk))
	{
		_stprintf(szErr, _T("关闭桌面失败:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
}
