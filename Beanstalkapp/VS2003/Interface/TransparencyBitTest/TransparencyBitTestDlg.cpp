// TransparencyBitTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransparencyBitTest.h"
#include "TransparencyBitTestDlg.h"
#include ".\transparencybittestdlg.h"

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


// CTransparencyBitTestDlg �Ի���



CTransparencyBitTestDlg::CTransparencyBitTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransparencyBitTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransparencyBitTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransparencyBitTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CTransparencyBitTestDlg ��Ϣ�������

BOOL CTransparencyBitTestDlg::OnInitDialog()
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
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTransparencyBitTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTransparencyBitTestDlg::OnPaint() 
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
HCURSOR CTransparencyBitTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTransparencyBitTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
	HBITMAP			hBitmap		= NULL;
	CClientDC		dc(this);

	long			lTime		= GetTickCount();	

	hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)); 

	CDC l_MaskDC; 
	l_MaskDC.CreateCompatibleDC(&dc); 
	CBitmap l_MaskBitmap; 
	l_MaskBitmap.CreateBitmap( 129, 180, 1, 1, NULL ); 
	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject( &l_MaskBitmap ); 

	CDC l_MemoryDC; 
	l_MemoryDC.CreateCompatibleDC( &dc ); 
	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject( CBitmap::FromHandle( hBitmap ) ); 
	COLORREF l_crOldBack = l_MemoryDC.SetBkColor( RGB( 255, 0, 255 ) ); 
//	l_MemoryDC.TextOut(0, 0, _T("ONE"));
	l_MaskDC.BitBlt( 0, 0, 129, 180, &l_MemoryDC, 0, 0, SRCCOPY ); 

	dc.BitBlt( 0, 0, 129, 180, &l_MemoryDC, 0, 0, SRCINVERT ); 
	dc.BitBlt( 0, 0, 129, 180, &l_MaskDC, 0, 0, SRCAND ); 
	dc.BitBlt( 0, 0, 129, 180, &l_MemoryDC, 0, 0, SRCINVERT ); 

	l_MemoryDC.SelectObject( l_pOldMemoryBitmap ); 
	l_MaskDC.SelectObject( l_pOldMaskBitmap ); 

	l_MaskDC.DeleteDC();
	l_MemoryDC.DeleteDC();
	::DeleteObject(hBitmap);

	lTime = GetTickCount() - lTime;
	CString str;
	str.Format(_T("����ʱ��:%6d����"), lTime);
	dc.TextOut(0, 190, str);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(0, 0, _T("ONE"));
}

void CTransparencyBitTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//TransparentBlt
	HBITMAP		hBitmap		= NULL;
	CClientDC	dc(this);
	CDC			memdc;

	long			lTime		= GetTickCount();	

	hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	memdc.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = memdc.SelectObject(CBitmap::FromHandle(hBitmap));

	::TransparentBlt(dc.m_hDC, 129, 0, 129, 180, memdc.m_hDC, 0, 0, 129, 180, (int)RGB(0xff, 0x00, 0xff));

	memdc.SelectObject(pOldBitmap);
	memdc.DeleteDC();
	::DeleteObject(hBitmap);
	lTime = GetTickCount() - lTime;

	CString str;
	str.Format(_T("����ʱ��:%6d����"), lTime);
	dc.TextOut(129, 190, str);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(129, 0, _T("TWO"));
}
