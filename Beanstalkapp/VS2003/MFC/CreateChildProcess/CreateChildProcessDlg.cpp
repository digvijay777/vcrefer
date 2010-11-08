// CreateChildProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateChildProcess.h"
#include "CreateChildProcessDlg.h"
#include ".\createchildprocessdlg.h"
#include "ProcessListDlg.h"
#include <shlwapi.h>
#include <Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CCreateChildProcessDlg 对话框



CCreateChildProcessDlg::CCreateChildProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateChildProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateChildProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateChildProcessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_SHOWPROCESSLIST, OnBnClickedBtShowprocesslist)
	ON_BN_CLICKED(IDC_BT_SELECT, OnBnClickedBtSelect)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CCreateChildProcessDlg 消息处理程序

BOOL CCreateChildProcessDlg::OnInitDialog()
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

void CCreateChildProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCreateChildProcessDlg::OnPaint() 
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
HCURSOR CCreateChildProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateChildProcessDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
// 	OnOK();
}

void CCreateChildProcessDlg::OnBnClickedBtShowprocesslist()
{
	CProcessListDlg		dlg;
	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemInt(IDC_ET_PROCESSID, dlg.m_dwProcessID);
	}
}

void CCreateChildProcessDlg::OnBnClickedBtSelect()
{
	CFileDialog		dlg(TRUE, "*.exe", "", 4 | 2, "可执行文件(*.exe)|*.exe|所有文件(*.*)|*.*||", this);
	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_ET_EXEPATH, dlg.GetPathName());
	}
}
//////////////////////////////////////////////////////////////////////////
// 数据结构体
#pragma pack(push)
#pragma pack(1)
struct _RemoteThreadInfo
{
	CHAR	szCmd[1024];
	int		nErrCode;

	void*	fnCreateProcess;
	void*	fnCloseHandle;
	void*	fnGetLastError;
};
#pragma pack(pop)
// 回调函数
#pragma runtime_checks("scu", off)
#pragma code_seg("sharecode")
static DWORD WINAPI _RemoteThreadCallFn(LPVOID lpParameter)
{
#ifdef _DEBUG
	__asm int 3
#endif

	STARTUPINFO				vSI	= {0};
	PROCESS_INFORMATION		vPI	= {0};

	_asm
	{
		mov		ebx, [lpParameter]
		lea		eax, [vPI]
		push	eax				// &vPI
		lea		eax, [vSI]
		push	eax				// &vSI
		xor		ecx, ecx
		push	ecx				// lpCurrentDir
		push	ecx				// lpEnvironment
		push	ecx				// dwCreationFlags
		push	ecx				// bInheritHandles
		push	ecx				// lpThreadAttributes
		push	ecx				// lpProcessAttributes
		lea		eax, [ebx+_RemoteThreadInfo::szCmd]
		push	eax				// lpCmd
		push	ecx				// lpApplication
		mov		edx, [ebx+_RemoteThreadInfo::fnCreateProcess]
		call	edx				// CreateProcess
		test	eax, eax
		jz		leableErr
		lea		edi, [vPI]
		mov		eax, [edi+PROCESS_INFORMATION::hProcess]
		mov		edx, [ebx+_RemoteThreadInfo::fnCloseHandle]
		push	eax
		call	edx
		mov		eax, [edi+PROCESS_INFORMATION::hThread]
		push	eax
		call	edx
		jmp		leableReturn
leableErr:
		mov		edx, [ebx+_RemoteThreadInfo::fnGetLastError]
		call	edx
		lea		edi, [ebx+_RemoteThreadInfo::nErrCode]
		mov		[edi], eax
leableReturn:
	}
	return 0;
}
#pragma code_seg("sharecode")
static void WINAPI _RemoteThreadCallFn_End(){}
#pragma runtime_checks("scu", restore)
//////////////////////////////////////////////////////////////////////////
/*
 *	创建进程
 *
 *  添加时间:2009-11-8
 */
void CCreateChildProcessDlg::OnBnClickedButton3()
{
	DWORD			dwPorcessID				= 0;
	TCHAR			szExePath[MAX_PATH]		= {0};
	BOOL			bRes					= FALSE;

	//////////////////////////////////////////////////////////////////////////
	// 验证输入
	dwPorcessID = GetDlgItemInt(IDC_ET_PROCESSID);
	GetDlgItemText(IDC_ET_EXEPATH, szExePath, MAX_PATH);
	if(0 == dwPorcessID)
	{
		AfxMessageBox("请选择父进程");
		return;
	}
	if(!PathFileExists(szExePath))
	{
		AfxMessageBox("请输入正确的可执行文件路径");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 创建进程
	DWORD		dwSize		= 0;
	HANDLE		hProcess	= NULL;
	CString		strErr;
	void*			fnRemoteThread	= 0;

	fnRemoteThread = (void *)_RemoteThreadCallFn;
	dwSize = (DWORD)_RemoteThreadCallFn_End - (DWORD)_RemoteThreadCallFn;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ | PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, dwPorcessID);
	if(NULL == hProcess)
	{
		strErr.Format("打开进程失败:%d", GetLastError());
		AfxMessageBox(strErr);
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 枚举进程内的模块
	HANDLE				hModuleSnapshot		= NULL;
	MODULEENTRY32		module				= {0};
	_RemoteThreadInfo	param				= {0};
	int					nModule				= 1;

	hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPorcessID);
	module.dwSize = sizeof(MODULEENTRY32);
	bRes = Module32First(hModuleSnapshot, &module);
	while(FALSE != bRes)
	{
		if(stricmp(module.szModule, "kernel32.dll") == 0)
		{
			// GetLastError
			param.fnGetLastError = (void *)GetProcAddress(module.hModule, "GetLastError");
			param.fnCreateProcess = (void *)GetProcAddress(module.hModule, "CreateProcessA");
			param.fnCloseHandle = (void *)GetProcAddress(module.hModule, "CloseHandle");
			nModule--;
		}
		if(nModule <= 0)
			break;
		bRes = Module32Next(hModuleSnapshot, &module);
	}
	CloseHandle(hModuleSnapshot);
	if(NULL == param.fnCloseHandle || NULL == param.fnGetLastError || NULL == param.fnCreateProcess)
	{
		AfxMessageBox("得到模块失败");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 分配内存和写入数据
	DWORD			dwResSize		= 0;
	void*			pProcMem		= NULL;
	int				nLastError		= 0;

	pProcMem = (void *)VirtualAllocEx(hProcess, NULL, dwSize + sizeof(_RemoteThreadInfo), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(NULL == pProcMem)
	{
		strErr.Format("分配内存失败:%d", GetLastError());
		AfxMessageBox(strErr);
		return;
	}
	do 
	{
		bRes = WriteProcessMemory(hProcess, pProcMem, fnRemoteThread, dwSize, &dwResSize);
		if(FALSE == bRes)
		{
			strErr.Format("写内存失败:%d", GetLastError());
			break;
		}
		strcpy(param.szCmd, szExePath);
		bRes = WriteProcessMemory(hProcess, (void *)((BYTE *)pProcMem + dwSize), &param, 
			sizeof(_RemoteThreadInfo), &dwResSize);
		if(FALSE == bRes)
		{
			strErr.Format("再次写入内存失败:%d", GetLastError());
			break;
		}

		// 调用远程线程
		DWORD			dwThreadID		= 0;
		HANDLE			hRemoteThread	= NULL;

		bRes = FALSE;
		hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pProcMem, 
			(LPVOID)((BYTE *)pProcMem + dwSize), 0, &dwThreadID);
		if(NULL == hRemoteThread)
		{
			strErr.Format("创建进程失败:%d", GetLastError());
			break;
		}

		dwResSize = WaitForSingleObject(hRemoteThread, INFINITE);

		// 获取返回值
		bRes = ReadProcessMemory(hProcess, (void *)((BYTE *)pProcMem + dwSize),
			&param, sizeof(_RemoteThreadInfo), &dwResSize);
		if(FALSE == bRes)
		{
			strErr.Format("读取内存失败:%d", GetLastError());
			break;
		}
		nLastError = param.nErrCode;
		if(0 == nLastError)
			bRes = TRUE;
		strErr.Format("远程线程返回:%d", nLastError);
	} while (0);
	VirtualFreeEx(hProcess, pProcMem, 0, MEM_RELEASE);
	// 返回
	if(TRUE != bRes)
	{
		AfxMessageBox(strErr);
	}
}
