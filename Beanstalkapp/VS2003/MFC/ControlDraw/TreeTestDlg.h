#pragma once
#include "afxcmn.h"
#include "SkyTreeCtrl.h"

// CTreeTestDlg dialog

class CTreeTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTreeTestDlg)

public:
	CTreeTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTreeTestDlg();

// Dialog Data
	enum { IDD = IDD_TREECTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CSkyTreeCtrl m_ctlTree;
	afx_msg void OnBnClickedButton1();

	HTREEITEM	m_hTreeItem;
	CFolderTree m_ctlFolder;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
