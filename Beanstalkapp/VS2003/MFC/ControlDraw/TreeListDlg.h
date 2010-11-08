#pragma once
#include "treelistctrl.h"


// CTreeListDlg dialog

class CTreeListDlg : public CDialog
{
	DECLARE_DYNAMIC(CTreeListDlg)

public:
	CTreeListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTreeListDlg();

// Dialog Data
	enum { IDD = IDD_TREELISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeListCtrl m_treelist;
	CImageList		m_imglist;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
