#pragma once
#include "saferepairctrl.h"


// CSysRepairDlg dialog

class CSysRepairDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysRepairDlg)

public:
	CSysRepairDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSysRepairDlg();

// Dialog Data
	enum { IDD = IDD_SysRepair };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	CSafeRepairCtrl m_ctlSafe;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtntest();
	virtual BOOL OnInitDialog();
};
