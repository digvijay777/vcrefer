// RemoteThreadDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteThreadDlg.h"
#include "RemoteThreadDlgDlg.h"
#include ".\remotethreaddlgdlg.h"
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


// CRemoteThreadDlgDlg 对话框



CRemoteThreadDlgDlg::CRemoteThreadDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteThreadDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteThreadDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRemoteThreadDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_REMOTEPTHREAD, OnBnClickedBtRemotepthread)
END_MESSAGE_MAP()


// CRemoteThreadDlgDlg 消息处理程序

BOOL CRemoteThreadDlgDlg::OnInitDialog()
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

void CRemoteThreadDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRemoteThreadDlgDlg::OnPaint() 
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
HCURSOR CRemoteThreadDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void WINAPI CRemoteThreadDlgDlg::ThreadRemoteFn(LPVOID lpParameter)
{
	STARTUPINFO			si		= {0};
	PROCESS_INFORMATION	pi		= {0};
	BOOL				bRes	= FALSE;
	CHAR				szPath[] = {'C', ':', '\\', 'W', 'i', 'n', 'd', 'o', 'w', 's',
		'\\', 'n', 'o', 't', 'e', 'p', 'a', 'd', '.', 'e',
		'x', 'e', 0};
/*
	STARTUPINFO			si		= {0};
	PROCESS_INFORMATION	pi		= {0};
	BOOL				bRes	= FALSE;
	CHAR				szPath[] = {'C', ':', '\\', 'W', 'i', 'n', 'd', 'o', 'w', 's',
										'\\', 'n', 'o', 't', 'e', 'p', 'a', 'd', '.', 'e',
										'x', 'e', 0};
	typedef BOOL (WINAPI *FUN)(LPCTSTR, LPTSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
		BOOL, DWORD, LPVOID, LPCTSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION);
	FUN funCreateProcess = (FUN)lpParameter;

	si.cb = sizeof(STARTUPINFO);
	bRes = funCreateProcess(szPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
*/
/*
		__asm{
			push dword ptr[pi]
			push dword ptr[si]
			push 0 ; NULL
			push 0 ; NULL
			push 0 ; FALSE
			push 0 ; NULL
			push 0 ; NULL
			push 0 ; NULL
			push szPath
			call lpParameter
			ret
		}*/
	
// 	else
// 	{
// 		CHAR		szMsg[1024]		= {0};
// 		CHAR		szPrompt[]		= {'S', 't', 'a', 'r', 't', 32, 'P', 'r', 
// 										'o', 'c', 'e', 's', 's', 32, 'F', 'a', 'i', 'l', 'e',
// 										'd', ':', '%' ,'d'};
// 		CHAR		szTitle[]		= {'E', 'r', 'r', 'o', 'r'};
// 
// 		sprintf(szMsg, szPrompt, GetLastError());
// 		::MessageBox(NULL, szMsg, szTitle, MB_OK | MB_ICONERROR);
// 	}
// 	return 0;
}
void WINAPI CRemoteThreadDlgDlg::ThreadRemoteFn_End(){}

#pragma pack(push)
#pragma pack(1)
struct RemoteParam
{
	CHAR					szExePath[MAX_PATH];
	PROCESS_INFORMATION		pi;
	STARTUPINFO				si;

	void*					lpCreateProcess;
	void*					lpCloseHandle;
};
#pragma pack(pop)
#pragma code_seg ("shareddata")
static DWORD WINAPI ThreadRemoteFn_share(LPVOID lpParameter)
{
	//CreateProcess(szPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
/*
	__asm{
		//push param->pi
		mov		ebx, lpParameter
		lea		eax, [ebx + RemoteParam::pi] 
		push	eax			// &pi
		lea		eax, [ebx + RemoteParam::si]
		push	eax			// &si
		push	0			// NULL
		push	0			// NULL
		push	0			// 0
		push	0			// FALSE
		push	0			// NULL
		push	0			// NULL
		push	0			// NULL
		lea		eax, [ebx]
		push	eax			// szPath
		mov		edx, [ebx + RemoteParam::lpCreateProcess]
		call	edx
	}*/
	
	RemoteParam		*param		= (RemoteParam *)lpParameter;
	BOOL			bRes		= FALSE;

	typedef BOOL (WINAPI *FUN1)(LPCTSTR, LPTSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
		BOOL, DWORD, LPVOID, LPCTSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION);
	typedef BOOL (WINAPI *FUN2)(HANDLE);

	FUN1 funCreateProcess = (FUN1)param->lpCreateProcess;
	FUN2 funCloseHandle	= (FUN2)param->lpCloseHandle;

	bRes = funCreateProcess(param->szExePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &param->si, &param->pi);
	if(TRUE == bRes)
	{
		funCloseHandle(param->pi.hProcess);
		funCloseHandle(param->pi.hThread);
	}
	return 0;
}

#pragma code_seg ("shareddata")
static void WINAPI ThreadRemoteFn_End_share(){}

void CRemoteThreadDlgDlg::OnBnClickedBtRemotepthread()
{
	// 读取文件 
	BYTE*			pFileData		= NULL;
	DWORD			dwSize			= 0;
	DWORD			dwResSize		= 0;
	BOOL			bRes			= 0;

// 	pFileData = (BYTE*)ThreadRemoteFn;
// 	dwSize = (DWORD)ThreadRemoteFn_End - (DWORD)ThreadRemoteFn;
	pFileData = (BYTE*)ThreadRemoteFn_share;
	dwSize = (DWORD)ThreadRemoteFn_End_share - (DWORD)ThreadRemoteFn_share;
	
	// 写进程数据
	int				nProcessID;
	HANDLE			hProcess;
	RemoteParam		param			= {0};

	nProcessID = GetDlgItemInt(IDC_ET_PROCESSID);
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ | PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD/* | PROCESS_CREATE_PROCESS*/,
		FALSE, nProcessID);
	do 
	{
		if(NULL == hProcess)
			break;

		// 枚举进程内的模块
		HANDLE			hToolSnap		= NULL;
		MODULEENTRY32	module			= {0};
		DWORD			dwKernelStart	= 0;

		hToolSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nProcessID);
		if(INVALID_HANDLE_VALUE == hToolSnap)
			break;
		module.dwSize = sizeof(MODULEENTRY32);
		bRes = Module32First(hToolSnap, &module);
		while(bRes)
		{
			if(strcmp(module.szModule, "kernel32.dll") == 0)
			{
				//dwKernelStart = (DWORD)(__int64)module.hModule;
				param.lpCreateProcess = (void *)GetProcAddress(module.hModule, "CreateProcessA");
				param.lpCloseHandle = (void *)GetProcAddress(module.hModule, "CloseHandle");
				break;
			}
			bRes = Module32Next(hToolSnap, &module);
		}
		CloseHandle(hToolSnap);
		if(NULL == param.lpCreateProcess || NULL == param.lpCloseHandle)
			break;
		// 远程分配内存
		VOID*		pProcessMem			= NULL;
		DWORD		dwThreadID			= 0;
		HANDLE		hThread				= NULL;

		pProcessMem = ::VirtualAllocEx(hProcess, 0, dwSize + sizeof(RemoteParam), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if(NULL == pProcessMem)
			break;
		bRes = WriteProcessMemory(hProcess, pProcessMem, pFileData, dwSize, &dwResSize);
		strcpy(param.szExePath, "C:\\Windows\\notepad.exe");
		bRes = WriteProcessMemory(hProcess, (void*)((BYTE*)pProcessMem + dwSize), &param, sizeof(RemoteParam), &dwResSize);
		hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pProcessMem, (LPVOID)((BYTE*)pProcessMem + dwSize), 0, &dwThreadID);
		if(NULL != hThread)
			WaitForSingleObject(hThread, INFINITE);
		::VirtualFreeEx(hProcess, pProcessMem, dwSize, MEM_RELEASE);
	} while (0);
}
