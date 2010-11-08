// MultiDesktopDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiDesktop.h"
#include "MultiDesktopDlg.h"
#include ".\multidesktopdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiDesktopDlg �Ի���
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


// CMultiDesktopDlg ��Ϣ�������

BOOL CMultiDesktopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiDesktopDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CMultiDesktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultiDesktopDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// ����������
	TCHAR		szDesktop[MAX_PATH]	= {0};
	TCHAR		szErr[MAX_PATH]		= {0};

	GetDlgItemText(IDC_ET_NEWDESKTOP, szDesktop, MAX_PATH);
	if(_tcslen(szDesktop) < 1)
	{
		AfxMessageBox("��������������");
		return;
	}
	
	HDESK		hDesk;
	DWORD		dwAccess = DESKTOP_ENUMERATE | DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP;

	hDesk = ::CreateDesktop(szDesktop, NULL, NULL, 0, dwAccess, NULL);
	if(NULL == hDesk)
	{
		_stprintf(szErr, _T("��������ʧ��:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
	// ����explorer����
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
	// �л�����
	TCHAR		szDesktop[MAX_PATH]	= {0};
	TCHAR		szErr[MAX_PATH]		= {0};
	TCHAR*		pUnderline			= NULL;
	
	m_list.GetText(m_list.GetCurSel(), szDesktop);
	if(0 == szDesktop[0])
	{
		AfxMessageBox("��ѡ������");
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
		_stprintf(szErr, _T("������ʧ��:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	// ö�����洰��
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
// 		_stprintf(szErr, _T("�����߳�����ʧ��:%d"), GetLastError());
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
		AfxMessageBox("��ѡ������");
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
		_stprintf(szErr, _T("������ʧ��:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	// ö�����洰��
	// 	HWND		hExplorer	= NULL;
	// 
	// 	EnumDesktopWindows(hDesk, EnumWindowsProc, (LPARAM)&hExplorer);
	// 	//
	// 	CloseDesktop(hDesk);
	// 	return;
	if(FALSE == CloseDesktop(hDesk))
	{
		_stprintf(szErr, _T("�ر�����ʧ��:%d"), GetLastError());
		AfxMessageBox(szErr);
		return;
	}
}
