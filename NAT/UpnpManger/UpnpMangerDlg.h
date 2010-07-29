// UpnpMangerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "upnp.h"
#include "upnp2.h"

// CUpnpMangerDlg 对话框
class CUpnpMangerDlg : public CDialog
{
// 构造
public:
	CUpnpMangerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPNPMANGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	VOID	EnableOptButton(BOOL bEnabled = TRUE);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	MyUPnP			m_upnp;
	CListCtrl		m_list;
	CEdit			m_ip;
	afx_msg void OnBnClickedBtRefurish();
	afx_msg void OnBnClickedBtAdd();
	afx_msg void OnBnClickedBtDelete();
};
