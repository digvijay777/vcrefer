// FileTreeTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "FileTreeTestDlg.h"
#include ".\filetreetestdlg.h"

#include <shellapi.h>

#pragma comment(lib, "shell32.lib")
// CFileTreeTestDlg dialog

IMPLEMENT_DYNAMIC(CFileTreeTestDlg, CDialog)
CFileTreeTestDlg::CFileTreeTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTreeTestDlg::IDD, pParent)
{
}

CFileTreeTestDlg::~CFileTreeTestDlg()
{
}

void CFileTreeTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CFileTreeTestDlg, CDialog)
END_MESSAGE_MAP()


// CFileTreeTestDlg message handlers

BOOL CFileTreeTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tree.InitFolder(IDB_TREEICON, IDB_FOLDERICON);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFileTreeTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_DROPFILES == pMsg->message)
	{
		OnDropFiles(pMsg->hwnd, (HDROP)pMsg->wParam);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFileTreeTestDlg::OnDropFiles( HWND hWnd, HDROP hDropInfo )
{
	if(hWnd == ::GetDlgItem(GetSafeHwnd(), IDC_ST_GROUP))
	{
		CHAR		szFile[MAX_PATH]		= {0};

		::DragQueryFile(hDropInfo, 0, szFile, sizeof(szFile));
		SetDlgItemText(IDC_ST_PATH, szFile);
		// Í¼±ê
		SHFILEINFO		info		= {0};

		SHGetFileInfo(szFile, 0, &info, sizeof(info), SHGFI_ICON);

		((CStatic*)GetDlgItem(IDC_ST_ICON))->SetIcon(info.hIcon);
	}
}