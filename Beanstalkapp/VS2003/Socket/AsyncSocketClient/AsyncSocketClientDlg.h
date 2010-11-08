// AsyncSocketClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CAsyncSocketClientDlg 对话框
class CAsyncSocketClientDlg : public CDialog
{
// 构造
public:
	CAsyncSocketClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ASYNCSOCKETCLIENT_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CListBox m_msg;
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnBnClickedBtClose();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnClose();
};
