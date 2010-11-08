// HTTPDownlogTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "../../../ExtClass/Net/DownFile.h"

// CHTTPDownlogTestDlg �Ի���
class CHTTPDownlogTestDlg : public CDialog, public CDownFileBase
{
// ����
public:
	CHTTPDownlogTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPDOWNLOGTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CProgressCtrl	m_progress;
private:	
	bool			m_signed;		
public:
	static DWORD WINAPI DownThread(LPVOID pParameter);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	virtual void NotifyUI(int nType, LPVOID lpParameter);
};
