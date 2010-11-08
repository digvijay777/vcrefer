#pragma once
#include "afxwin.h"
#include "ToolTipStatic.h"

// CToolTipCtrlDlg dialog

class CToolTipCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolTipCtrlDlg)

public:
	CToolTipCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolTipCtrlDlg();

// Dialog Data
	enum { IDD = IDD_TOOLTIPCTRLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CToolTipCtrl		m_tooltip;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CToolTipStatic  m_ctlStatic;
};
