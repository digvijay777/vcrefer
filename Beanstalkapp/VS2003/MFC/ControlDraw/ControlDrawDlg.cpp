// ControlDrawDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "ControlDrawDlg.h"
#include ".\controldrawdlg.h"

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
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
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


// CControlDrawDlg �Ի���



CControlDrawDlg::CControlDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDrawDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CControlDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TAB, m_tab);
	DDX_Control(pDX, IDC_BUTTON1, m_tranbtn);
	// 	DDX_Control(pDX, IDC_STATIC_g, m_groupbox);
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_LIST1, (CWnd &)m_ctlPlugList);
}

BEGIN_MESSAGE_MAP(CControlDrawDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_STATIC_TAB, OnTabCtrlSelectChange)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
END_MESSAGE_MAP()


// CControlDrawDlg ��Ϣ�������

BOOL CControlDrawDlg::OnInitDialog()
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
	m_tab.InitTabCtrl(&CSize(50, 25));
// 	m_tab.InsertItem(0, "hello");
// 	m_tab.InsertItem(1, "hi");
// 	m_tab.InsertItem(1, "Ha ha ha ha ha ha");
	m_tab.SetCurSel(0);

	m_radio1.LoadBitmap(IDB_RADIO);
	CBitmap		pBitmap;

	pBitmap.LoadBitmap(IDB_CHECK);
	m_ctlPlugList.SetImageList(IDB_CHECK, IDB_PROMPT);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CControlDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CControlDrawDlg::OnPaint() 
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
HCURSOR CControlDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CControlDrawDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CControlDrawDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CControlDrawDlg::OnTabCtrlSelectChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString str;
	str.Format("ѡ�����%d", m_tab.GetCurSel());
	SetDlgItemText(IDC_STATIC_TABSEL, str);
}
BOOL CControlDrawDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect	rect;

// 	GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// 
// 	pDC->FillSolidRect(&rect, RGB(0xff, 0xff, 0xff));
// 
// 	return TRUE;
// 
// 	return CDialog::OnEraseBkgnd(pDC);
/*
	static BITMAP bitmap = {0};
	if(NULL == m_bmpBk.m_hObject)
	{
		m_bmpBk.LoadBitmap(IDB_BITMAP_BK);
		if(NULL == m_bmpBk.m_hObject)
			return CDialog::OnEraseBkgnd(pDC);
		m_bmpBk.GetBitmap(&bitmap);
	}
	CDC		memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap*  pBmp = memDC.SelectObject(&m_bmpBk);
// 	CRect	rect;
	GetClientRect(&rect);
	//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
*/
	return CDialog::OnEraseBkgnd(pDC);
}

void CControlDrawDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CControlDrawDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
}

void CControlDrawDlg::OnBnClickedButton2()
{
	m_ctlPlugList.AddPlug("hi", "F:\\SVN\\common\\Object\\dsafdasfasdc\\fdaas\\fsdafa\\���", "", "");
	m_ctlPlugList.AddPlug("hello", "type", "company", "path");
}

void CControlDrawDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_ctlPlugList.ClearPlug();
}

void CControlDrawDlg::OnBnClickedButton4()
{
	CString			strText;

	GetDlgItemText(IDC_BUTTON4, strText);
	if("ȫѡ" == strText)
	{
		m_ctlPlugList.CheckAll(TRUE);
		SetDlgItemText(IDC_BUTTON4, "ȫ��ѡ");
	}
	else
	{
		m_ctlPlugList.CheckAll(FALSE);
		SetDlgItemText(IDC_BUTTON4, "ȫѡ");
	}
}

void CControlDrawDlg::OnBnClickedButton5()
{
	m_ctlPlugList.AddPrompt("û�н���ɨ��ɨ��", "����һ��С��");
}
