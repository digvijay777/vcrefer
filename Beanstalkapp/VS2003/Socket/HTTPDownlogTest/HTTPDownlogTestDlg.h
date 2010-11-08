// HTTPDownlogTestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "../../../ExtClass/Net/DownFile.h"

// CHTTPDownlogTestDlg 对话框
class CHTTPDownlogTestDlg : public CDialog, public CDownFileBase
{
// 构造
public:
	CHTTPDownlogTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTTPDOWNLOGTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
