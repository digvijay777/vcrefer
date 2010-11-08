// ClarityWndDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../../../ExtClass/mfc/SelectWndBtn.h"
#include "afxcmn.h"

// CClarityWndDlg 对话框
class CClarityWndDlg : public CDialog
{
// 构造
public:
	CClarityWndDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLARITYWND_DIALOG };

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
	afx_msg LRESULT OnEndSelectWnd(WPARAM wParam, LPARAM lParam);

	CSelectWndBtn m_btnSertch;
	CSliderCtrl m_Alpha;
	HWND		m_hAlphaWnd;
	afx_msg void OnNMThemeChangedSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
