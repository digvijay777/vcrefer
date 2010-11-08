#pragma once


// CModifyLicenseDlg dialog
#define IDMODIFY		100
#define IDDELETE		101
#define IDINVALID		102

class CModifyLicenseDlg : public CDialog
{
	DECLARE_DYNAMIC(CModifyLicenseDlg)

public:
	CModifyLicenseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyLicenseDlg();

// Dialog Data
	enum { IDD = IDD_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_szID;
	CString m_szUser;
	CString m_szTel;
	CString m_szCount;
	CString m_szCurCount;
	CString m_szTime;
	CString m_szChange;
	CString m_szAuthor;
	CString m_szAddTime;
	CString	m_szTitle;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	virtual BOOL OnInitDialog();
private:
	CString m_szOldCount;
	CString m_szOldTime;
};
