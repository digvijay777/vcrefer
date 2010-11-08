// GetPasswdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetPasswd.h"
#include "GetPasswdDlg.h"
#include ".\getpasswddlg.h"
#include <Tlhelp32.h>

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


// CGetPasswdDlg �Ի���



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


// CGetPasswdDlg ��Ϣ�������

BOOL CGetPasswdDlg::OnInitDialog()
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
	m_selectwnd.SetCursor(IDC_CURSOR2);
	CHAR*		pStr = "����һ��С���ߣ���Ҫ��;��������ȡ�������ES_PASSWORD�����ı�������"
						"[������ʼ]��ť�����϶���굽Ҫ��ȡ�Ĵ����Ͽգ�Ȼ���ɿ����Ϳ�����";
	SetDlgItemText(IDC_STATIC_S, pStr);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGetPasswdDlg::OnPaint() 
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
HCURSOR CGetPasswdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGetPasswdDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	OnOK();
}

/*
 *	��ť��֪ͨ����ʱ��ʾ, ȡ�����ڵı���
 *
 *  ���ʱ��:2009-11-7
 */
LRESULT CGetPasswdDlg::OnSelectedWnd(WPARAM wParam, LPARAM lParam)
{
	HWND		hWnd			= (HWND)wParam;
	CString		strTitle;
	int			nLength			= 0;

	if(!IsWindow(hWnd))
		return 0;

	// ȡ���ڱ���ʧ��
	DWORD				dwStyle		= 0;
	CString				strMsg;

	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if(0 == dwStyle)
	{
		strMsg.Format("ȡ��������ʧ��:%d", GetLastError());
		AfxMessageBox(strMsg);
	}
	else
	{
		// �ǲ��������
		if((ES_PASSWORD & dwStyle) > 0)
		{
			if(FALSE == GetPasswordWndTitle(hWnd, strTitle))
			{
				strMsg.Format("�õ����봰���ı�ʧ��:%d", GetLastError());
				AfxMessageBox(strMsg);
			}
			else
			{
				AfxMessageBox(strTitle);
			}
		}
		else // ���������ı�
		{
			nLength = ::GetWindowText(hWnd, strTitle.GetBufferSetLength(1024), 1024);
			strTitle.ReleaseBuffer();
			AfxMessageBox(strTitle);
		}
	}
	return 0;
}

/*
 *	Զ�̽����ڵ��̻߳ص�����
 *
 *  ���ʱ��:2009-11-7
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
 *	���������������е��̻߳ص�����
 *
 *  ���ʱ��:2009-11-7
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
 *	�õ��������ı�
 *
 *  ���ʱ��:2009-11-7
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
	// ����������
    HANDLE			hProcess		= NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ | PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD/* | PROCESS_CREATE_PROCESS*/,
		FALSE, dwProcessID);
	if(NULL == hProcess)
		return	FALSE;

	//////////////////////////////////////////////////////////////////////////
	// ö�ٽ����ڵ�ģ��
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
	// ��ʼ����Զ���߳�
	DWORD			dwSize			= 0;
	DWORD			dwResSize		= 0;
	void*			fnRemoteThread	= 0;

	fnRemoteThread = (void *)GetPasswdRemoteThreadFn;
	dwSize = (DWORD)GetPasswdRemoteThreadFn_End - (DWORD)GetPasswdRemoteThreadFn;
	
	// �����ڴ��д������
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
		
		// ����Զ���߳�
		DWORD			dwThreadID		= 0;
		HANDLE			hRemoteThread	= NULL;

		bRes = FALSE;
		hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pProcMem, 
			(LPVOID)((BYTE *)pProcMem + dwSize), 0, &dwThreadID);
		if(NULL == hRemoteThread)
			break;

		dwResSize = WaitForSingleObject(hRemoteThread, INFINITE);

		// ��ȡ����ֵ
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
	// ����
	SetLastError(nLastError);
	return bRes;
}

BOOL CGetPasswdDlg::OnQueryOpen()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialog::OnQueryOpen();
}
