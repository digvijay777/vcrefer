#pragma once

#ifndef UM_SELECTWNDBTNBASE
#define UM_SELECTWNDBTNBASE				(WM_USER+0x200)
#endif

// 开始拖动鼠标进行选择窗体 wParam: 0		lParam: 0
#define UM_BEGINSELECTWND				UM_SELECTWNDBTNBASE+1
// 结束拖动鼠标 wParam: HWND(目标)	   lParam: 0
#define UM_ENDSELECTWND					UM_SELECTWNDBTNBASE+2	
// 鼠标移动中选择窗体改变时通知: wParam: HWND	lParam: 0
#define UM_CHANGESELECTWND				UM_SELECTWNDBTNBASE+3



class CSelectWndBtn : public CButton
{
	DECLARE_DYNAMIC(CSelectWndBtn)

public:
	CSelectWndBtn();
	virtual ~CSelectWndBtn();
private:
	BOOL		m_bLButtonDown;
	HWND		m_hOldSelWnd;
	HCURSOR		m_hCursor;
	HCURSOR		m_hOldCursor;
public:
	int			m_nRevertWidth;		// 选中的窗体边框反色宽度
private:
	void InvertRectangle(HDC hDC, LPRECT lpRect, int nWidth);
	void InvertWindowFrame(HWND hWnd, int nWidth);
	void NotifyParent(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
protected:
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	void SetCursor(UINT  nCursorID);
};


