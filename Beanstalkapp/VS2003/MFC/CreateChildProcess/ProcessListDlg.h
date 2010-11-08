#pragma once
#include "afxcmn.h"


// CProcessListDlg 对话框

class CProcessListDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessListDlg)
public:
	DWORD			m_dwProcessID;
public:
	CProcessListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessListDlg();

// 对话框数据
	enum { IDD = IDD_PROCESSLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void FillList();
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;

};
