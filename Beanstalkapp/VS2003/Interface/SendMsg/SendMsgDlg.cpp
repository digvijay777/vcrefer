// SendMsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendMsg.h"
#include "SendMsgDlg.h"
#include ".\sendmsgdlg.h"

#include "ProcessWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendMsgDlg �Ի���



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


// CSendMsgDlg ��Ϣ�������

BOOL CSendMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

// 	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CenterWindow();
	
	EnableSendWndGrups(false);
	SetDlgItemText(IDC_RT_MSG, _T(""));

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   

	((CButton*)GetDlgItem(IDC_WND_HANDLE_HEX))->SetCheck(1);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSendMsgDlg::OnPaint() 
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
		//�������
		GetDlgItemText(IDC_WND_HANDLE, szText, 49);
		if(_tcslen(szText) > 0)
		{
			if(((CButton*)GetDlgItem(IDC_WND_HANDLE_HEX))->GetCheck())
			{
				//ʮ������
				hWnd = (HWND)_tcstol(szText, NULL, 16);
			}
			else
			{
				//ʮ����
				hWnd = (HWND)_tcstol(szText, NULL, 10);
			}
			break;
		}
		//������������
		GetDlgItemText(IDC_WND_CLASS, szText, 49);
		if(_tcslen(szText) > 0)
		{
			hWnd = ::FindWindow(szText, NULL);
			break;
		}
		//�����������
		GetDlgItemText(IDC_WND_TITLE, szText, 49);
		if(_tcslen(szText) > 0)
		{
			hWnd = ::FindWindow(NULL, szText);
			break;
		}
	} while (false);

	if(NULL == hWnd || !::IsWindow(hWnd))
	{
		SetDlgItemText(IDC_RT_MSG, _T("�Ҳ���ָ������"));
		return;
	}

	CWnd* pWnd = CWnd::FromHandle(hWnd);
	if(NULL == pWnd)
	{
		SetDlgItemText(IDC_RT_MSG, _T("ָ�����ھܾ�����"));
		return;
	}
	//��������
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
			SetDlgItemText(IDC_RT_STATUS, _T("��ȡʧ��"));
			break;
		}
	}
	else
		SetDlgItemText(IDC_RT_STATUS, _T("��ȡʧ��"));
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
		//ʮ������
		msg = _tcstol(szText, NULL, 16);
	}
	else
	{
		//ʮ����
		msg = _tcstol(szText, NULL, 10);
	}
	GetDlgItemText(IDC_RT_HANDLE, szText, 49);
	hWnd = (HWND)_tcstol(szText, NULL, 16);
	if(!IsWindow(hWnd))
	{
		EnableSendWndGrups(false);
		SetDlgItemText(IDC_RT_MSG, _T("���ھ����Ч��������ָ�����ڱ��ر�"));
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
	// �����б�
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
