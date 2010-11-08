// IEPasswdWatchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IEPasswdWatch.h"
#include "IEPasswdWatchDlg.h"
#include ".\iepasswdwatchdlg.h"
#include "ExternIEOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIEPasswdWatchDlg �Ի���


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


// CIEPasswdWatchDlg ��Ϣ�������

BOOL CIEPasswdWatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIEPasswdWatchDlg::OnPaint() 
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
