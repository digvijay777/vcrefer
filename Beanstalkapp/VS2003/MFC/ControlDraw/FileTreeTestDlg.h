#pragma once
#include "FileTree.h"

// CFileTreeTestDlg dialog

class CFileTreeTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileTreeTestDlg)

public:
	CFileTreeTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileTreeTestDlg();

// Dialog Data
	enum { IDD = IDD_FILETREETESTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnDropFiles( HWND hWnd, HDROP hDropInfo );

	CFileTree		 m_tree;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
