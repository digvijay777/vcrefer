#pragma once
#include "Upnp2.h"

// CAddUpnpDlg dialog

class CAddUpnpDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddUpnpDlg)

public:
	CAddUpnpDlg(MyUPnP* pUpnp, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddUpnpDlg();

// Dialog Data
	enum { IDD = IDD_ADDUPNPDLG };

	MyUPnP*		m_upnp;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
