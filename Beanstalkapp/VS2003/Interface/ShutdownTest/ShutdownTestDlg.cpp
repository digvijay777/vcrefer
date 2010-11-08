// ShutdownTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShutdownTest.h"
#include "ShutdownTestDlg.h"
#include ".\shutdowntestdlg.h"

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


// CShutdownTestDlg �Ի���



CShutdownTestDlg::CShutdownTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShutdownTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShutdownTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShutdownTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CShutdownTestDlg ��Ϣ�������

BOOL CShutdownTestDlg::OnInitDialog()
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

void CShutdownTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CShutdownTestDlg::OnPaint() 
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
HCURSOR CShutdownTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShutdownTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

void CShutdownTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//��ȡ�ػ�Ȩ��
	GetPrivilege();
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);

	// ��һ�ַ���
// 	HANDLE				hProcess	= NULL;
// 	HANDLE				hToken		= NULL;
// 	TOKEN_PRIVILEGES	Privileges	= {0};
// 	LUID				luid		= {0};
// 
// 	hProcess = GetCurrentProcess();
// 	// ����Ϊ�򿪵�ǰ���̶Ի�
// 	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
// 	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
// 
// 	Privileges.PrivilegeCount			= 1;
// 	Privileges.Privileges[0].Luid		= luid; 
// 	Privileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 
// 
// 	AdjustTokenPrivileges(hToken, FALSE, &Privileges, NULL, NULL, NULL);
// 	// ����
// 	if(FALSE == InitiateSystemShutdownEx(NULL, "ϵͳά��", 0, FALSE, TRUE, 
// 		SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_SECURITY | SHTDN_REASON_FLAG_PLANNED))
// 	{
// 		NLog::FolderFileLog(DBG_LOGDIR, DBG_LOGFILE, "Reboot InitiateSystemShutdownEx error:%d", GetLastError());
// 	}
}



void CShutdownTestDlg::GetPrivilege(void)
{
	HANDLE				hProcess	= NULL;
	HANDLE				hToken		= NULL;
	TOKEN_PRIVILEGES	Privileges	= {0};
	LUID				luid		= {0};
	
	hProcess = GetCurrentProcess();
	//����Ϊ�򿪵�ǰ���̶Ի�
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);

	Privileges.PrivilegeCount			= 1;
	Privileges.Privileges[0].Luid		= luid; 
	Privileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 

	AdjustTokenPrivileges(hToken, FALSE, &Privileges, NULL, NULL, NULL);
}

void CShutdownTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	GetPrivilege();
	ExitWindowsEx(EWX_REBOOT, 0);
}

void CShutdownTestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	GetPrivilege();
	ExitWindowsEx(EWX_LOGOFF, 0);

	//InitiateSystemShutdownEx();δ��¼�û�
}

void CShutdownTestDlg::RebootSystem()
{
	HANDLE				hProcess	= NULL;
	HANDLE				hToken		= NULL;
	TOKEN_PRIVILEGES	Privileges	= {0};
	LUID				luid		= {0};

	hProcess = GetCurrentProcess();
	// ����Ϊ�򿪵�ǰ���̶Ի�
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);

	Privileges.PrivilegeCount			= 1;
	Privileges.Privileges[0].Luid		= luid; 
	Privileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED; 

	AdjustTokenPrivileges(hToken, FALSE, &Privileges, NULL, NULL, NULL);
	// ����
	if(FALSE == InitiateSystemShutdownEx(NULL, "ϵͳά��", 0, FALSE, TRUE, 
		SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_SECURITY | SHTDN_REASON_FLAG_PLANNED))
	{
		TRACE("Reboot InitiateSystemShutdownEx error:%d", GetLastError());
	}
}