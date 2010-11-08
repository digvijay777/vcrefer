// ProcessAsUserDlg.cpp : 实现文件
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
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CProcessAsUserDlg 对话框



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


// CProcessAsUserDlg 消息处理程序

BOOL CProcessAsUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessAsUserDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CProcessAsUserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
 *	高级用户运行
 *
 *  添加时间:2009-10-29
 */
void CProcessAsUserDlg::OnBnClickedButton1()
{
	BOOL		bRet		= FALSE;
	EnablePrivilege(SE_DEBUG_NAME);
	//////////////////////////////////////////////////////////////////////////
	// 系统用户
	HANDLE		hSystem		= NULL;
	HANDLE		hSysToken	= NULL;

	hSystem = OpenSystemProcess();
	bRet = OpenProcessToken(hSystem, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &hSysToken);
	bRet = ImpersonateLoggedOnUser(hSysToken);

	//////////////////////////////////////////////////////////////////////////
	// 创建进程
	HANDLE		hToken		= NULL;
	STARTUPINFO si			= {0};
	PROCESS_INFORMATION pi	= {0};

 	si.cb = sizeof(STARTUPINFO);
  	si.lpDesktop = "Winsta0\\Default";
// 	HDESK		hDesk		= GetThreadDesktop(GetCurrentThreadId());
// 	si.lpDesktop = hDesk;
	
	bRet = LogonUser("Administrator", NULL, "pqqwan", LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken);
	bRet = CreateProcessAsUser(hToken, "C:\\WINDOWS\\NOTEPAD.EXE", "", NULL, NULL, TRUE, 
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, // 进程参数
		NULL, NULL, &si, &pi);
// 	bRet = CreateProcessWithLogonW(L"Administrator", NULL, L"pqqwan", LOGON_WITH_PROFILE,
// 		L"C:\\WINDOWS\\NOTEPAD.EXE", L"", CREATE_DEFAULT_ERROR_MODE,
// 		NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(hToken);
	//////////////////////////////////////////////////////////////////////////
	// 返回原来
	CloseHandle(hSysToken);
	RevertToSelf();
}

/*
 *	得到系统用户进程句柄
 *
 *  添加时间:2009-10-29
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
