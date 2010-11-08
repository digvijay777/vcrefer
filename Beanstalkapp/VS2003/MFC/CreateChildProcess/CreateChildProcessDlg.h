// CreateChildProcessDlg.h : 头文件
//

#pragma once


// CCreateChildProcessDlg 对话框
class CCreateChildProcessDlg : public CDialog
{
// 构造
public:
	CCreateChildProcessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CREATECHILDPROCESS_DIALOG };

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
	afx_msg void OnBnClickedBtShowprocesslist();
	afx_msg void OnBnClickedBtSelect();
	afx_msg void OnBnClickedButton3();
};
