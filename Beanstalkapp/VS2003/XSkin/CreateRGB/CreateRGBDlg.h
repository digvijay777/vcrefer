// CreateRGBDlg.h : 头文件
//

#pragma once


// CCreateRGBDlg 对话框
class CCreateRGBDlg : public CDialog
{
// 构造
public:
	CCreateRGBDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CREATERGB_DIALOG };

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
	int m_nModule;
	CString m_path;
	afx_msg void OnBnClickedBtScan();
	afx_msg void OnBnClickedOk();
	int m_nRows;
	BOOL m_nAllScan;
	BOOL m_bEnter;
	int m_nShowModule;
};
