#pragma once

#include "Resource.h"
#include "afxcmn.h"
#include "ImgTabCtrl.h"
// CTabCtrlDlg dialog

class CTabCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CTabCtrlDlg)

public:
	CTabCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabCtrlDlg();

// Dialog Data
	enum { IDD = IDD_TABCTRLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CImgTabCtrl		m_ctlTab;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
