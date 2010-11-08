#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUploadFileDlg dialog

class CUploadFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadFileDlg)

public:
	CUploadFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUploadFileDlg();

// Dialog Data
	enum { IDD = IDD_UPLOADFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
private:
	int				m_nHeight;
	CString			m_strSvrPath;
private:
	BOOL		UploadFile();
	void		AddLog(LPSTR lpLog, BOOL bMsg = FALSE);
	void		AddErrLog(LPSTR lpPreLog, BOOL bMsg = FALSE);
	void		EnableOptWindow(BOOL bEnble);
	void		InitVirusList();
	void		SendAtCommand(LPSTR lpIP);
public:
	static DWORD CALLBACK _UploadFileThread(LPVOID lpParameter);
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedFindvirus();
	CEdit m_ctlLog;
	CString			m_strIP;
	CListBox m_ctlVirus;
	afx_msg void OnBnClickedRestartservice();
};
