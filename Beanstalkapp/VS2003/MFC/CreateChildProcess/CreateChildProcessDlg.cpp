// CreateChildProcessDlg.cpp : ʵ���ļ�
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


// CCreateChildProcessDlg �Ի���



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


// CCreateChildProcessDlg ��Ϣ�������

BOOL CCreateChildProcessDlg::OnInitDialog()
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCreateChildProcessDlg::OnPaint() 
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
HCURSOR CCreateChildProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateChildProcessDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CFileDialog		dlg(TRUE, "*.exe", "", 4 | 2, "��ִ���ļ�(*.exe)|*.exe|�����ļ�(*.*)|*.*||", this);
	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_ET_EXEPATH, dlg.GetPathName());
	}
}
//////////////////////////////////////////////////////////////////////////
// ���ݽṹ��
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
// �ص�����
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
 *	��������
 *
 *  ���ʱ��:2009-11-8
 */
void CCreateChildProcessDlg::OnBnClickedButton3()
{
	DWORD			dwPorcessID				= 0;
	TCHAR			szExePath[MAX_PATH]		= {0};
	BOOL			bRes					= FALSE;

	//////////////////////////////////////////////////////////////////////////
	// ��֤����
	dwPorcessID = GetDlgItemInt(IDC_ET_PROCESSID);
	GetDlgItemText(IDC_ET_EXEPATH, szExePath, MAX_PATH);
	if(0 == dwPorcessID)
	{
		AfxMessageBox("��ѡ�񸸽���");
		return;
	}
	if(!PathFileExists(szExePath))
	{
		AfxMessageBox("��������ȷ�Ŀ�ִ���ļ�·��");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��������
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
		strErr.Format("�򿪽���ʧ��:%d", GetLastError());
		AfxMessageBox(strErr);
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// ö�ٽ����ڵ�ģ��
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
		AfxMessageBox("�õ�ģ��ʧ��");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// �����ڴ��д������
	DWORD			dwResSize		= 0;
	void*			pProcMem		= NULL;
	int				nLastError		= 0;

	pProcMem = (void *)VirtualAllocEx(hProcess, NULL, dwSize + sizeof(_RemoteThreadInfo), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(NULL == pProcMem)
	{
		strErr.Format("�����ڴ�ʧ��:%d", GetLastError());
		AfxMessageBox(strErr);
		return;
	}
	do 
	{
		bRes = WriteProcessMemory(hProcess, pProcMem, fnRemoteThread, dwSize, &dwResSize);
		if(FALSE == bRes)
		{
			strErr.Format("д�ڴ�ʧ��:%d", GetLastError());
			break;
		}
		strcpy(param.szCmd, szExePath);
		bRes = WriteProcessMemory(hProcess, (void *)((BYTE *)pProcMem + dwSize), &param, 
			sizeof(_RemoteThreadInfo), &dwResSize);
		if(FALSE == bRes)
		{
			strErr.Format("�ٴ�д���ڴ�ʧ��:%d", GetLastError());
			break;
		}

		// ����Զ���߳�
		DWORD			dwThreadID		= 0;
		HANDLE			hRemoteThread	= NULL;

		bRes = FALSE;
		hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pProcMem, 
			(LPVOID)((BYTE *)pProcMem + dwSize), 0, &dwThreadID);
		if(NULL == hRemoteThread)
		{
			strErr.Format("��������ʧ��:%d", GetLastError());
			break;
		}

		dwResSize = WaitForSingleObject(hRemoteThread, INFINITE);

		// ��ȡ����ֵ
		bRes = ReadProcessMemory(hProcess, (void *)((BYTE *)pProcMem + dwSize),
			&param, sizeof(_RemoteThreadInfo), &dwResSize);
		if(FALSE == bRes)
		{
			strErr.Format("��ȡ�ڴ�ʧ��:%d", GetLastError());
			break;
		}
		nLastError = param.nErrCode;
		if(0 == nLastError)
			bRes = TRUE;
		strErr.Format("Զ���̷߳���:%d", nLastError);
	} while (0);
	VirtualFreeEx(hProcess, pProcMem, 0, MEM_RELEASE);
	// ����
	if(TRUE != bRes)
	{
		AfxMessageBox(strErr);
	}
}
