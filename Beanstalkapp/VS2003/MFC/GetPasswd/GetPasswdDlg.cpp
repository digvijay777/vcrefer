// GetPasswdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GetPasswd.h"
#include "GetPasswdDlg.h"
#include ".\getpasswddlg.h"
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


// CGetPasswdDlg 对话框



CGetPasswdDlg::CGetPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetPasswdDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_BEGIN, m_selectwnd);
}

BEGIN_MESSAGE_MAP(CGetPasswdDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(UM_ENDSELECTWND, OnSelectedWnd)
	ON_WM_QUERYOPEN()
END_MESSAGE_MAP()


// CGetPasswdDlg 消息处理程序

BOOL CGetPasswdDlg::OnInitDialog()
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
	m_selectwnd.SetCursor(IDC_CURSOR2);
	CHAR*		pStr = "这是一个小工具，主要用途是用来获取窗体具用ES_PASSWORD风格的文本。单击"
						"[单击开始]按钮，并拖动鼠标到要获取的窗体上空，然后松开鼠标就可以了";
	SetDlgItemText(IDC_STATIC_S, pStr);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CGetPasswdDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetPasswdDlg::OnPaint() 
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
HCURSOR CGetPasswdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGetPasswdDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
// 	OnOK();
}

/*
 *	按钮有通知窗体时显示, 取出窗口的标题
 *
 *  添加时间:2009-11-7
 */
LRESULT CGetPasswdDlg::OnSelectedWnd(WPARAM wParam, LPARAM lParam)
{
	HWND		hWnd			= (HWND)wParam;
	CString		strTitle;
	int			nLength			= 0;

	if(!IsWindow(hWnd))
		return 0;

	// 取窗口标题失败
	DWORD				dwStyle		= 0;
	CString				strMsg;

	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if(0 == dwStyle)
	{
		strMsg.Format("取窗口属性失败:%d", GetLastError());
		AfxMessageBox(strMsg);
	}
	else
	{
		// 是不是密码框
		if((ES_PASSWORD & dwStyle) > 0)
		{
			if(FALSE == GetPasswordWndTitle(hWnd, strTitle))
			{
				strMsg.Format("得到密码窗口文本失败:%d", GetLastError());
				AfxMessageBox(strMsg);
			}
			else
			{
				AfxMessageBox(strTitle);
			}
		}
		else // 不是密码文本
		{
			nLength = ::GetWindowText(hWnd, strTitle.GetBufferSetLength(1024), 1024);
			strTitle.ReleaseBuffer();
			AfxMessageBox(strTitle);
		}
	}
	return 0;
}

/*
 *	远程进程内的线程回调函数
 *
 *  添加时间:2009-11-7
 */
#pragma pack(push)
#pragma pack(1)
struct StructGetPasswdRemoteThread
{
	CHAR		szTitle[MAX_PATH];
	HWND		hWnd;
	DWORD		dwRes;

	void*		fnGetWindowText;
	void*		fnGetLastError;
};
#pragma pack(pop)
/*
 *	在密码框进程内运行的线程回调函数
 *
 *  添加时间:2009-11-7
 */
#pragma code_seg ("sharecode")
#pragma runtime_checks("scu", off)
static void WINAPI GetPasswdRemoteThreadFn(LPVOID lpParameter)
{
#ifdef _DEBUG
	__asm int 3
#endif

	__asm
	{
// 		int		3
		mov		eax, MAX_PATH
		mov		ebx, [lpParameter]
		push	eax				// nMaxCount
		lea		eax, [ebx + StructGetPasswdRemoteThread::szTitle]
		push	eax				// lpString
		mov		eax, [ebx + StructGetPasswdRemoteThread::hWnd]
		push	eax				// hWnd
		mov		edx, [ebx + StructGetPasswdRemoteThread::fnGetWindowText]
		call	edx				// GetWindowTextA
		mov		edx, [ebx + StructGetPasswdRemoteThread::fnGetLastError]
		call	edx				// GetLastError
		lea		ecx, [ebx + StructGetPasswdRemoteThread::dwRes]
		mov		[ecx], eax		// dwRes
		xor		eax, eax		// return 0
	}
}
#pragma runtime_checks("scu", restore)
#pragma code_seg ("sharecode")
static void WINAPI GetPasswdRemoteThreadFn_End(){}
#pragma code_seg ()
/*
 *	得到密码框的文本
 *
 *  添加时间:2009-11-7
 */
BOOL CGetPasswdDlg::GetPasswordWndTitle(HWND hWnd, CString& strTitle)
{
	DWORD		dwProcessID		= 0;
	BOOL		bRes			= FALSE;
	int			nLastError		= 0;

	::GetWindowThreadProcessId(hWnd, &dwProcessID);
	if(0 == dwProcessID)
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	// 打开密码框进程
    HANDLE			hProcess		= NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ | PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD/* | PROCESS_CREATE_PROCESS*/,
		FALSE, dwProcessID);
	if(NULL == hProcess)
		return	FALSE;

	//////////////////////////////////////////////////////////////////////////
	// 枚举进程内的模块
	HANDLE			hModuleSnapshot		= NULL;
	MODULEENTRY32	module				= {0};
	StructGetPasswdRemoteThread	param	= {0};
	int				nModule				= 2;
	
	hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	module.dwSize = sizeof(MODULEENTRY32);
	bRes = Module32First(hModuleSnapshot, &module);
	while(FALSE != bRes)
	{
		if(stricmp(module.szModule, "kernel32.dll") == 0)
		{
			// GetLastError
			param.fnGetLastError = (void *)GetProcAddress(module.hModule, "GetLastError");
			nModule--;
		}
		if(stricmp(module.szModule, "user32.dll") == 0)
		{
			// GetWindowTextA
			param.fnGetWindowText = (void *)GetProcAddress(module.hModule, "GetWindowTextA");
			nModule--;
		}
		if(nModule <= 0)
			break;
		bRes = Module32Next(hModuleSnapshot, &module);
	}
	CloseHandle(hModuleSnapshot);
	if(NULL == param.fnGetWindowText || NULL == param.fnGetLastError)
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	// 开始调用远程线程
	DWORD			dwSize			= 0;
	DWORD			dwResSize		= 0;
	void*			fnRemoteThread	= 0;

	fnRemoteThread = (void *)GetPasswdRemoteThreadFn;
	dwSize = (DWORD)GetPasswdRemoteThreadFn_End - (DWORD)GetPasswdRemoteThreadFn;
	
	// 分配内存和写入数据
	void*			pProcMem		= NULL;

	pProcMem = (void *)VirtualAllocEx(hProcess, NULL, dwSize + sizeof(StructGetPasswdRemoteThread), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(NULL == pProcMem)
		return FALSE;
	do 
	{
		bRes = WriteProcessMemory(hProcess, pProcMem, fnRemoteThread, dwSize, &dwResSize);
		if(FALSE == bRes)
			break;
		param.hWnd = hWnd;
		bRes = WriteProcessMemory(hProcess, (void *)((BYTE *)pProcMem + dwSize), &param, 
			sizeof(StructGetPasswdRemoteThread), &dwResSize);
		if(FALSE == bRes)
			break;
		
		// 调用远程线程
		DWORD			dwThreadID		= 0;
		HANDLE			hRemoteThread	= NULL;

		bRes = FALSE;
		hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pProcMem, 
			(LPVOID)((BYTE *)pProcMem + dwSize), 0, &dwThreadID);
		if(NULL == hRemoteThread)
			break;

		dwResSize = WaitForSingleObject(hRemoteThread, INFINITE);

		// 获取返回值
		bRes = ReadProcessMemory(hProcess, (void *)((BYTE *)pProcMem + dwSize),
			&param, sizeof(StructGetPasswdRemoteThread), &dwResSize);
		if(FALSE == bRes)
			break;
		strTitle = param.szTitle;
		nLastError = param.dwRes;
		if(0 == nLastError || strTitle.GetLength() > 0)
			bRes = TRUE;
	} while (0);
	VirtualFreeEx(hProcess, pProcMem, 0, MEM_RELEASE);
	// 返回
	SetLastError(nLastError);
	return bRes;
}

BOOL CGetPasswdDlg::OnQueryOpen()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialog::OnQueryOpen();
}
