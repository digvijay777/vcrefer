// MultiDesktopDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMultiDesktopDlg 对话框
class CMultiDesktopDlg : public CDialog
{
// 构造
public:
	CMultiDesktopDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MULTIDESKTOP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void RefurbishDesktop();
	static BOOL CALLBACK EnumDesktopProc(LPTSTR lpszDesktop, LPARAM lParam);
	static BOOL CALLBACK EnumWindowStationProc(LPTSTR lpszWindowStation, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

public:
	afx_msg void OnBnClickedOk();
	CListBox m_list;
	afx_msg void OnBnClickedBtNewdesktop();
	afx_msg void OnBnClickedBtSwitchdesktop();
	afx_msg void OnBnClickedButton1();
};
