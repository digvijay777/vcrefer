// UserProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserProcess.h"
#include "UserProcessDlg.h"
#include ".\userprocessdlg.h"

#include <atlsecurity.h>

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


// CUserProcessDlg �Ի���



CUserProcessDlg::CUserProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserProcessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CUserProcessDlg ��Ϣ�������

BOOL CUserProcessDlg::OnInitDialog()
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

void CUserProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUserProcessDlg::OnPaint() 
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
HCURSOR CUserProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUserProcessDlg::OnBnClickedButton1()
{
	BOOL		bRet		= FALSE;
	HANDLE		hToken		= 0;

// 	bRet = LogonUser("Administrator", NULL, "123456", LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken);
// 	if(bRet)
// 	{
// // 		bRet = ImpersonateLoggedOnUser(hToken);
// 		//CloseHandle(hToken);
// 	}
	CAccessToken  token;

	bRet = token.LogonUser("Administrator", NULL, "123456", LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT);
// 	hToken = 0;
// 	 bRet = OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_DEFAULT, FALSE, &hToken);
// 	 RevertToSelf();
     if(bRet)
	 {
		 STARTUPINFO				si				= {0};
		 PROCESS_INFORMATION		pi				= {0};
		 CHAR						szCommand[]		= "\"c:\\inetpub\\wwwroot\\skynet\\bin\\7za.exe\" e -y -o\"c:\\inetpub\\wwwroot\\vc\\V30628\\\" \"c:\\inetpub\\wwwroot\\vc\\V30628\\V30628.zip\"";

// 		 bRet = CreateProcessWithLogonW(L"Administrator", NULL, L"123456", LOGON_WITH_PROFILE, NULL, szCommand, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi);
		 //bRet = CreateProcessAsUserW(&token, NULL, szCommand, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		bRet = token.CreateProcessAsUser(NULL, szCommand, &pi, &si);
	 }
}
