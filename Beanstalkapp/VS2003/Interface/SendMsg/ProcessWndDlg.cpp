// ProcessWndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SendMsg.h"
#include "ProcessWndDlg.h"
#include ".\processwnddlg.h"

#include <Tlhelp32.h>

#pragma comment(lib, "Kernel32.lib")
// CProcessWndDlg dialog

CProcessWndDlg* CProcessWndDlg::s_pThis = NULL;

IMPLEMENT_DYNAMIC(CProcessWndDlg, CDialog)
CProcessWndDlg::CProcessWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessWndDlg::IDD, pParent)
{
	s_pThis = this;
	m_hSelWnd = NULL;
}

CProcessWndDlg::~CProcessWndDlg()
{
}

void CProcessWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TR_PROCESSWND, m_tree);
}


BEGIN_MESSAGE_MAP(CProcessWndDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_REFRUSH, OnBnClickedBtRefrush)
	ON_BN_CLICKED(IDC_BT_SELECTWND, OnBnClickedBtSelectwnd)
END_MESSAGE_MAP()

BOOL CProcessWndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 树形框
	m_imgList.Create(IDB_BITMAP1, 15, 2, RGB(0xC0, 0xC0, 0xC0));
	m_tree.SetImageList(&m_imgList, TVSIL_NORMAL);
	InitializeTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CProcessWndDlg message handlers

void CProcessWndDlg::OnBnClickedOk()
{
	//OnOK();
}

void CProcessWndDlg::OnBnClickedBtRefrush()
{
	// 刷新
	m_tree.DeleteAllItems();
	m_Process.clear();

	InitializeTree();
}

/*
 *	初始化控件
 */
void CProcessWndDlg::InitializeTree()
{

// 	m_tree.InsertItem("tttt");
// 	m_tree.InsertItem("ssss");
// 	return;

	HANDLE				hToolHelp32		= NULL;
	PROCESSENTRY32		processEntry	= {0};

	hToolHelp32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(INVALID_HANDLE_VALUE == hToolHelp32)
		return;
	processEntry.dwSize = sizeof(PROCESSENTRY32);
	if(TRUE == Process32First(hToolHelp32, &processEntry))
	{
		do 
		{
			HTREEITEM		hItem		= NULL;
			CHAR			szShow[200]	= {0};
			sprintf(szShow, "[%08x] %s", processEntry.th32ProcessID, processEntry.szExeFile);
			hItem = m_tree.InsertItem(szShow);
			s_pThis->m_tree.SetItemImage(hItem, 1, 1);
			m_Process.insert(std::map<DWORD, HTREEITEM>::value_type(processEntry.th32ProcessID, hItem));
			//ExpendProcess(hItem, processEntry.th32ProcessID);
		} while (Process32Next(hToolHelp32, &processEntry));
	}
    CloseHandle(hToolHelp32);

	// 展开线程
	HANDLE			hToolThread			= NULL;
	THREADENTRY32	threadEntry			= {0};

	std::map<DWORD, HTREEITEM>::iterator		mapProcess		= m_Process.begin();
	hToolThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL); // 注意参数据运用
	if(INVALID_HANDLE_VALUE == hToolThread)
		return;
	threadEntry.dwSize = sizeof(THREADENTRY32);
	if(TRUE == Thread32First(hToolThread, &threadEntry))
	{
		do
		{
			if(0 == threadEntry.th32OwnerProcessID)
				continue;
			EnumThreadWindows(threadEntry.th32ThreadID, EnumThreadWndProc, (LPARAM)m_Process[threadEntry.th32OwnerProcessID]);
		}while(Thread32Next(hToolThread, &threadEntry));
	}
	CloseHandle(hToolThread);
}


BOOL CProcessWndDlg::EnumThreadWndProc(HWND hwnd, LPARAM lParam)
{
	CHAR		szTitle[256]	= {0};
	CHAR		szShow[1024]	= {0};
	CHAR		szClass[256]	= {0};

	::GetWindowText(hwnd, szTitle, 256);
	::GetClassName(hwnd, szClass, 256);
	sprintf(szShow, "[%08x] [%s] %s", hwnd, szClass, szTitle);
	HTREEITEM	hTree = s_pThis->m_tree.InsertItem(szShow, (HTREEITEM)lParam);
	s_pThis->m_tree.SetItemImage(hTree, 0, 0);
	s_pThis->m_tree.SetItemData(hTree, (DWORD_PTR)hwnd);
	EnumChildWindows(hwnd, EnumThreadWndProc, (LPARAM)hTree);
	return TRUE;
}
void CProcessWndDlg::OnBnClickedBtSelectwnd()
{
	HTREEITEM	hTree = m_tree.GetSelectedItem();
	DWORD_PTR	pdwData = m_tree.GetItemData(hTree);

	if(NULL != pdwData || (DWORD_PTR)-1 != pdwData)
	{
		if(IsWindow((HWND)pdwData))
		{
			m_hSelWnd = (HWND)pdwData;
			EndDialog(IDOK);
		}
	}
}
