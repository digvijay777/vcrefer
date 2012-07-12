// IconListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IconList.h"
#include "IconListDlg.h"

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


// CIconListDlg �Ի���




CIconListDlg::CIconListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIconListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIconListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIconListDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CIconListDlg ��Ϣ�������

BOOL CIconListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitGdiplus();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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
	m_ilc.m_imageRadio = ImageFromResouce(AfxGetInstanceHandle(), _T("PNG"),
		MAKEINTRESOURCE(IDR_PNG1));
	m_ilc.m_imageItemBk = ImageFromResouce(AfxGetInstanceHandle(), _T("PNG"),
		MAKEINTRESOURCE(IDR_PNG2));
	m_ilc.m_imageItemEt = ImageFromResouce(AfxGetInstanceHandle(), _T("PNG"),
		MAKEINTRESOURCE(IDR_PNG3));
	m_ilc.SubclassWindow(::GetDlgItem(GetSafeHwnd(), IDC_ICONLIST));
	// �������
	m_ilc.AddItem(0, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1)),
		L"��������", 500 + 0);
	m_ilc.AddItem(0, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON2)),
		L"�Ż�����", 500 + 1);
	m_ilc.AddItem(0, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON3)),
		L"©���޸�", 500 + 2);
	m_ilc.AddItem(1, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON4)),
		L"���ܴ�ȫ", 500 + 3);
	m_ilc.AddItem(1, LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON5)),
		L"����ܼ�", 500 + 4);
	m_ilc.UpdateGroup(0);
	m_ilc.UpdateGroup(1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CIconListDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIconListDlg::OnPaint()
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
//
HCURSOR CIconListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

