// SQLTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SQLTest.h"
#include "SQLTestDlg.h"
#include ".\sqltestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF")
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


// CSQLTestDlg �Ի���



CSQLTestDlg::CSQLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSQLTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSQLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSQLTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CSQLTestDlg ��Ϣ�������

BOOL CSQLTestDlg::OnInitDialog()
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

void CSQLTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSQLTestDlg::OnPaint() 
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
HCURSOR CSQLTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��������
void CSQLTestDlg::OnBnClickedButton1()
{
	_ConnectionPtr	m_spConnect;
	LPSTR			lpCon = "Provider=SQLOLEDB.1;User ID=sa;Password=123123;"
							"Persist Security Info=True;Initial Catalog=master;Network Address=192.168.1.22";
// 	LPSTR			lpSqlBk = "backup database small to disk='c:\\small2.dat' with init";
	LPSTR			lpSqlBk = "backup database small to disk='small4.dat' with init";
	LPSTR			lpSqlRe = "ALTER DATABASE small SET OFFLINE WITH ROLLBACK IMMEDIATE;"
							"restore database small from disk='c:\\small2.dat' with replace;"
							"ALTER DATABASE small SET ONLINE WITH ROLLBACK IMMEDIATE";
	CoInitialize(NULL);
	//�������ݿ�
	try{
		m_spConnect.CreateInstance(__uuidof(Connection));
		//m_spConnect->put_ConnectionTimeout(3000);
		m_spConnect->ConnectionTimeout = 3;
		m_spConnect->Open(lpCon, "", "", -1);

		AfxMessageBox("���ӳɹ�");
		m_spConnect->Execute(lpSqlBk, NULL, 0);
		AfxMessageBox("���ݳɹ�");
		// ���ӳɹ�
		m_spConnect->Close();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description(), MB_OK | MB_ICONERROR);
		return ;
	}
	CoUninitialize();
}
