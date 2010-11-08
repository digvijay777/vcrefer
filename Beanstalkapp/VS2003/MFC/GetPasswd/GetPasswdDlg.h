// GetPasswdDlg.h : 头文件
//

#pragma once
#include "../../../extclass/mfc/SelectWndBtn.h"


// CGetPasswdDlg 对话框
class CGetPasswdDlg : public CDialog
{
// 构造
public:
	CGetPasswdDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GETPASSWD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	
	BOOL GetPasswordWndTitle(HWND hWnd, CString& strTitle);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnSelectedWnd(WPARAM wParam, LPARAM lParam);
	CSelectWndBtn m_selectwnd;
	afx_msg BOOL OnQueryOpen();
};
