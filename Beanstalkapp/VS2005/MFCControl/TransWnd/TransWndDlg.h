// TransWndDlg.h : 头文件
//

#pragma once
#include "ShowDlg.h"

// CTransWndDlg 对话框
class CTransWndDlg : public CDialog
{
// 构造
public:
	CTransWndDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRANSWND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


private:
	Gdiplus::Image*		m_pImage;

public:
	static BOOL		UpdateLayeredImageWindow(Gdiplus::Image* pImage, CDC* pDC, CWnd* pWnd);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
