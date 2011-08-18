// TimeSelectDlg.h : 头文件
//

#pragma once
#include "TimeSelectCtrl.h"

// CTimeSelectDlg 对话框
class CTimeSelectDlg : public CDialog
{
// 构造
public:
	CTimeSelectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TIMESELECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CTimeSelectCtrl		m_timesel;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
