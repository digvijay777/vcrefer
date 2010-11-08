// ControlDrawDlg.h : 头文件
//

#pragma once
#include "minitabctrl.h"
#include "transparentbtn.h"
#include "afxwin.h"
#include "../../../ExtClass/mfc/RadioButton.h"
#include "../../../ExtClass/mfc/pluglistctrl.h"


// CControlDrawDlg 对话框
class CControlDrawDlg : public CDialog
{
// 构造
public:
	CControlDrawDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONTROLDRAW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CBitmap		m_bmpBk;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CMiniTabCtrl m_tab;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTabCtrlSelectChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
	CTransparentBtn m_tranbtn;
// 	CButton			m_groupbox;

	CRadioButton			m_radio1;
	afx_msg void OnBnClickedRadio1();
	CPlugListCtrl m_ctlPlugList;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
