// NoManifestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NoManifest.h"
#include "NoManifestDlg.h"
#include <shlobj.h>

#include "UACAutoRun.h"

#pragma comment(lib, "shell32.lib")

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


// CNoManifestDlg �Ի���




CNoManifestDlg::CNoManifestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoManifestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNoManifestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNoManifestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CNoManifestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CNoManifestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNoManifestDlg ��Ϣ�������

BOOL CNoManifestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CNoManifestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNoManifestDlg::OnPaint()
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
HCURSOR CNoManifestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNoManifestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
	if(IsUserAnAdmin())
	{
		MessageBox(_T("�ǹ���ԱȨ��"));
	}
	else
	{
		MessageBox(_T("���ǹ���ԱȨ��"));
	}
}

void CNoManifestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	HANDLE hToken = NULL; 
	TOKEN_PRIVILEGES tkp; 
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS,   &hToken)   ) 
	{ 
		if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid)   ) 
		{ 
			if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0x10, (PTOKEN_PRIVILEGES)NULL, 0))
				MessageBox(_T("Errored step:3"));
		} 
		else MessageBox(_T("Errored step:2"));
	} 
	else MessageBox(_T("Errored step:1"));
	if(hToken)
		CloseHandle(hToken); 
	//
// 	HANDLE		hToken		= NULL;
	HANDLE		hUser		= NULL;
	HANDLE		hUserToken	= NULL;
	BOOL		bRest		= FALSE;

	GetTokenByProcName(hToken, _T("winlogon.exe"));
	OpenProcessToken(hToken, MAXIMUM_ALLOWED, &hUser);

// 	DuplicateTokenEx(hUser, MAXIMUM_ALLOWED, NULL
// 		, SecurityIdentification, TokenPrimary, &hUserToken);

	bRest = ImpersonateLoggedOnUser(hUser);
	RunAsUAC();
	RevertToSelf();
}
