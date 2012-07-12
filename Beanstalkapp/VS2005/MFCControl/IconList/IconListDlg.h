// IconListDlg.h : 头文件
//

#pragma once
#include "IconList/IconListCtrl.h"
#include "afxwin.h"

// CIconListDlg 对话框
class CIconListDlg : public CDialog
{
// 构造
public:
	CIconListDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ICONLIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON			m_hIcon;
	CIconListCtrl	m_ilc;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUmNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_ck;
public:
	afx_msg void OnBnClickedCheck1();
};
