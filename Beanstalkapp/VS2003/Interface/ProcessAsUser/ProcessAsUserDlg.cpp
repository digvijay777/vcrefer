// ProcessAsUserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessAsUser.h"
#include "ProcessAsUserDlg.h"
#include ".\processasuserdlg.h"

#include <Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// #pragma comment(lib, "Toolhelp.lib")
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


// CProcessAsUserDlg �Ի���



CProcessAsUserDlg::CProcessAsUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessAsUserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessAsUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProcessAsUserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CProcessAsUserDlg ��Ϣ�������

BOOL CProcessAsUserDlg::OnInitDialog()
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

void CProcessAsUserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProcessAsUserDlg::OnPaint() 
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
HCURSOR CProcessAsUserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
 *	�߼��û�����
 *
 *  ���ʱ��:2009-10-29
 */
void CProcessAsUserDlg::OnBnClickedButton1()
{
	BOOL		bRet		= FALSE;
	EnablePrivilege(SE_DEBUG_NAME);
	//////////////////////////////////////////////////////////////////////////
	// ϵͳ�û�
	HANDLE		hSystem		= NULL;
	HANDLE		hSysToken	= NULL;

	hSystem = OpenSystemProcess();
	bRet = OpenProcessToken(hSystem, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &hSysToken);
	bRet = ImpersonateLoggedOnUser(hSysToken);

	//////////////////////////////////////////////////////////////////////////
	// ��������
	HANDLE		hToken		= NULL;
	STARTUPINFO si			= {0};
	PROCESS_INFORMATION pi	= {0};

 	si.cb = sizeof(STARTUPINFO);
  	si.lpDesktop = "Winsta0\\Default";
// 	HDESK		hDesk		= GetThreadDesktop(GetCurrentThreadId());
// 	si.lpDesktop = hDesk;
	
	bRet = LogonUser("Administrator", NULL, "pqqwan", LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken);
	bRet = CreateProcessAsUser(hToken, "C:\\WINDOWS\\NOTEPAD.EXE", "", NULL, NULL, TRUE, 
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, // ���̲���
		NULL, NULL, &si, &pi);
// 	bRet = CreateProcessWithLogonW(L"Administrator", NULL, L"pqqwan", LOGON_WITH_PROFILE,
// 		L"C:\\WINDOWS\\NOTEPAD.EXE", L"", CREATE_DEFAULT_ERROR_MODE,
// 		NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(hToken);
	//////////////////////////////////////////////////////////////////////////
	// ����ԭ��
	CloseHandle(hSysToken);
	RevertToSelf();
}

/*
 *	�õ�ϵͳ�û����̾��
 *
 *  ���ʱ��:2009-10-29
 */
HANDLE CProcessAsUserDlg::OpenSystemProcess() 
{
	HANDLE hSnapshot = NULL;
	HANDLE hProc     = NULL;

	try {{
		// Get a snapshot of the processes in the system
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == NULL)
			goto leave;

		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);

		// Find the "System" process
		BOOL fProcess = Process32First(hSnapshot, &pe32);
		while (fProcess && (lstrcmpi(pe32.szExeFile, TEXT("winlogon.exe")) != 0))
			fProcess = Process32Next(hSnapshot, &pe32);
		if (!fProcess)
			goto leave;    // Didn't find "System" process

		// Open the process with PROCESS_QUERY_INFORMATION access
		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE,
			pe32.th32ProcessID);
		if (hProc == NULL)
			goto leave;

	} leave:;
	} catch(...) {}

	// Cleanup the snapshot
	if (hSnapshot != NULL)
		CloseHandle(hSnapshot);

	return(hProc);
}

BOOL	CProcessAsUserDlg::EnablePrivilege(TCHAR * name, BOOL fEnable, HANDLE hToken)
{
	BOOL				bRetVal;
	int					nError;
	HANDLE				hNewToken = NULL;
	TOKEN_PRIVILEGES	priv = { 1, {0, 0, fEnable ? SE_PRIVILEGE_ENABLED : 0} };

	LookupPrivilegeValue(0, name, &priv.Privileges[0].Luid);
	if(hToken == NULL)
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hNewToken);
	else
		hNewToken = hToken;

	bRetVal = AdjustTokenPrivileges(hNewToken, FALSE, &priv, sizeof(priv), 0, 0);
	if(bRetVal == FALSE)
	{
		nError = ::GetLastError();
		if(hToken == NULL)	
			CloseHandle(hNewToken);

		::SetLastError(nError);
		return FALSE;
	}

	if(hToken == NULL)	
		CloseHandle(hNewToken);
	return TRUE;
}


BOOL	CProcessAsUserDlg::EnableProcPrivilege(DWORD PID)
{
	HANDLE					hProc, hToken; 
	BOOL					bRetVal;

	bRetVal = EnablePrivilege(SE_DEBUG_NAME);
	if(bRetVal == FALSE)
		return FALSE;

	if(PID == 0)
		return TRUE;

	hProc = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
	if(hProc == NULL)
		return FALSE;

	bRetVal = OpenProcessToken(hProc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	if(bRetVal == FALSE)
	{
		CloseHandle(hProc);
		return FALSE;
	}

	bRetVal = EnablePrivilege(SE_DEBUG_NAME, TRUE, hToken);
	CloseHandle(hToken);
	CloseHandle(hProc);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}
