#pragma once
#include "afxcmn.h"


// CProcessListDlg �Ի���

class CProcessListDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessListDlg)
public:
	DWORD			m_dwProcessID;
public:
	CProcessListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessListDlg();

// �Ի�������
	enum { IDD = IDD_PROCESSLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	void FillList();
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;

};
