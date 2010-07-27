// NetClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CNetClientDlg 对话框
class CNetClientDlg : public CDialog
{
// 构造
public:
	CNetClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NETCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CString			m_strINI;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	SOCKET		m_sockChart;

	void		ListenSocket(SOCKET sock);
	void		EnableChartWin(BOOL bConnect = FALSE);
public:
	static DWORD CALLBACK	ListenSocketThread(LPVOID lpParam);
	static DWORD CALLBACK	RecvFromSocketThread(LPVOID lpParam);
	CListBox m_list;
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnBnClickedBtSendnat();
};
