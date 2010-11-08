#pragma once
#include "afxwin.h"
#include "SkyCheckListBox.h"
#include "SkyScoreList.h"
#include "SkyScoreListEx.h"

// CCheckListBoxDlg dialog

class CCheckListBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckListBoxDlg)

public:
	CCheckListBoxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckListBoxDlg();

// Dialog Data
	enum { IDD = IDD_CHECKLISTBOXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
// 	CSkyCheckListBox m_ctlList;
// 	CSkyScoreList		m_ctlList;
	CSkyScoreListEx		m_ctlList;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedCancel();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
