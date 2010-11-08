// ClarityWndDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClarityWnd.h"
#include "ClarityWndDlg.h"
#include ".\claritywnddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClarityWndDlg �Ի���



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


// CClarityWndDlg ��Ϣ�������

BOOL CClarityWndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Alpha.SetRange(0, 255);
	m_Alpha.SetPos(100);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClarityWndDlg::OnPaint() 
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
	if("ȡ��͸��" == str)
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
		SetDlgItemText(IDOK, "����͸��");
		return;
	}

	// ͸������
	HWND			hWnd;

	m_hAlphaWnd = NULL;
	GetDlgItemText(IDC_EDIT1, str);
	sscanf(str.GetBuffer(), "0x%08X", (DWORD *)&(hWnd));

	if(!IsWindow(hWnd))
	{
		AfxMessageBox("������Ч����");
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
	SetDlgItemText(IDOK, "ȡ��͸��");
	m_hAlphaWnd = hWnd;
}

/*
 *	���ѡ��
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
