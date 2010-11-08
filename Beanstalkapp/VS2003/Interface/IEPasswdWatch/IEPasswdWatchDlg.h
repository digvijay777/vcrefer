// IEPasswdWatchDlg.h : 头文件
//

#pragma once


// CIEPasswdWatchDlg 对话框
class CIEPasswdWatchDlg : public CDialog
{
// 构造
public:
	CIEPasswdWatchDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IEPASSWDWATCH_DIALOG };

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
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
private:
	BOOL	m_bCapture;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
