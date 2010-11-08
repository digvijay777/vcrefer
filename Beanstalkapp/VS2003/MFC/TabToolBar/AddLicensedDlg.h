#pragma once
#include "afxwin.h"


// CAddLicensedDlg dialog

class CAddLicensedDlg : public CDialog
{
	DECLARE_DYNCREATE(CAddLicensedDlg)

public:
	CAddLicensedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddLicensedDlg();

// Dialog Data
	enum { IDD = IDD_ADD};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_szID;
	CString m_szUser;
	CString m_szTel;
	CString m_szClientCount;
	CString m_szMatureDate;
	CString m_szAuthor;
	int		m_nType;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCbGroup();
	CComboBox m_cbGroup;
private:
	HINSTANCE	m_hInstLicense;
};
