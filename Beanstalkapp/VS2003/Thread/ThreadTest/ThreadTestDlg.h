// ThreadTestDlg.h : 头文件
//

#pragma once


// CThreadTestDlg 对话框
class CThreadTestDlg : public CDialog
{
// 构造
public:
	CThreadTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_THREADTEST_DIALOG };

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
private:
	CWinThread*			m_pThread;
public:
	afx_msg void OnBnClickedBtCreatethread();
	afx_msg void OnBnClickedBtSendthreadmessage();
	afx_msg void OnBnClickedStEndthread();
};
