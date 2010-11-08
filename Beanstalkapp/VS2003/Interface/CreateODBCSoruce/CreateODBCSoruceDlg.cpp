// CreateODBCSoruceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateODBCSoruce.h"
#include "CreateODBCSoruceDlg.h"
#include ".\createodbcsorucedlg.h"

#include "odbcinst.h"
#pragma comment(lib, "odbccp32.lib")

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


// CCreateODBCSoruceDlg �Ի���



CCreateODBCSoruceDlg::CCreateODBCSoruceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateODBCSoruceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hBrush = CreateSolidBrush(RGB(0xff, 0xff, 0xff));
}

CCreateODBCSoruceDlg::~CCreateODBCSoruceDlg()
{
	if(NULL != m_hBrush)
		DeleteObject(m_hBrush);
}

void CCreateODBCSoruceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateODBCSoruceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCreateODBCSoruceDlg ��Ϣ�������

BOOL CCreateODBCSoruceDlg::OnInitDialog()
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

void CCreateODBCSoruceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCreateODBCSoruceDlg::OnPaint() 
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
HCURSOR CCreateODBCSoruceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateODBCSoruceDlg::OnBnClickedOk()
{
	char	szName[MAX_PATH]		= {0};
	char	szPath[MAX_PATH]		= {0};
	char	szFullPath[MAX_PATH]	= {0};

	GetDlgItemText(IDC_EDIT1, szPath, MAX_PATH);
	GetDlgItemText(IDC_EDIT2, szName, MAX_PATH);

	if(strlen(szName) == 0 || strlen(szPath) == 0)
	{
		AfxMessageBox("����д�����Ϣ");
		return;
	}

	strcpy(szFullPath, szPath);
	char*	pChar = strrchr(szPath, '\\');
	if(NULL != pChar)
		*pChar = 0;
	//����ODBC
	char	szDNS[1024]		= {0};
	sprintf(szDNS, "DSN=%s;DBQ=%s;DEFAULTDIR=%s;;", szName, szFullPath, szPath);
	for(int i = strlen(szDNS) - 1; i >=0; i--)
	{
		if(szDNS[i] == ';')
			szDNS[i] = 0;
	}
	bool bRet = SQLConfigDataSource(NULL, 
		ODBC_ADD_SYS_DSN, 
		"Microsoft Access Driver (*.mdb)\0",
		szDNS);
	AfxMessageBox(bRet?"��ӳɹ�":"���ʧ��");
}

HBRUSH CCreateODBCSoruceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(GetDlgItem(IDC_EDIT1)->m_hWnd == pWnd->m_hWnd)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_hBrush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CCreateODBCSoruceDlg::OnBnClickedButton1()
{
	CFileDialog fd(true, "*.mdb", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "���ݿ��ļ�(*.mdb)|*.mdb|�����ļ�(*.*)|*.*||", this);
	if(fd.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT1, fd.GetPathName());
	}
}
