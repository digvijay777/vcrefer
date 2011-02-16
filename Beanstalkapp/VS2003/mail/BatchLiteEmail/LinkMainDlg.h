#pragma once
#include "afxcmn.h"


// CLinkMainDlg dialog

class CLinkMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinkMainDlg)

public:
	CLinkMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLinkMainDlg();

// Dialog Data
	enum { IDD = IDD_LINKMAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	BOOL		m_bModify;
	afx_msg void OnBnClickedAdd();
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();

public:
	void OnLoadLinkMan();
	void OnSelectItem(int nItem);
//	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSaveadd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSavemodify();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
