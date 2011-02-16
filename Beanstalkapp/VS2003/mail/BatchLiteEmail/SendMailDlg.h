#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define UM_TRAYNOTIFY		WM_USER+100
// CSendMailDlg dialog

class CSendMailDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendMailDlg)

public:
	CSendMailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSendMailDlg();

// Dialog Data
	enum { IDD = IDD_SENDMAILDLG };

	CString			m_strTemplateUrl;
	HANDLE			m_hSendThread;
	BOOL			m_bCancel;

	static DWORD CALLBACK _ThreadProc(LPVOID lpParameter);

	void		EnableWorkItem(BOOL bWork);
	void		DoSendMail();
	void		ShowDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLinkmanager();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtAdd();
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	CListBox m_ltAttachment;
	CListBox m_ltLog;
	afx_msg void OnBnClickedBtDelete();
	CString m_strName;
	CString m_strEmail;
	CString m_strSvr;
	CString m_strUser;
	CString m_strPwd;
	CProgressCtrl m_ctlPos;
	afx_msg void OnBnClickedBtSend();
	virtual BOOL OnInitDialog();
	CString m_strTitle;
	afx_msg void OnBnClickedBtHide();
	afx_msg void OnBnClickedBtTest();
};
