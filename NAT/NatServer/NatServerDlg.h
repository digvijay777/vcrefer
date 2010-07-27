// NatServerDlg.h : 头文件
//

#pragma once
#include <vector>
#include "afxwin.h"

// CNatServerDlg 对话框
class CNatServerDlg : public CDialog
{
// 构造
public:
	CNatServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NATSERVER_DIALOG };

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
	afx_msg void OnBnClickedOk();

	void			OnListen();
	void			OnAccept(SOCKET sock);

	std::vector<SOCKET>			m_vctClient;
	static CNatServerDlg*		m_pThis;
	static DWORD CALLBACK		OnThreadListen(LPVOID lpParam);
	static DWORD CALLBACK		OnThreadAccept(LPVOID lpParam);
	CListBox m_list;
};
