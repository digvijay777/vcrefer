#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <gdiplus.h>

class CFloatingWnd : public CWnd
{
public:
	CFloatingWnd();
	virtual ~CFloatingWnd(void);

public:
	// CWindow的方法
	BOOL	Create(Gdiplus::Image* pImage, int nSplit);
	void	UpdateWindow();
	// 消息循环
	DECLARE_MESSAGE_MAP()
protected:
	Gdiplus::Image*			m_pBkImage;
	SIZE					m_size;
	RECT					m_rtClient;
	BYTE					m_bAlpha;

	struct {
		POINT				pt;
		BOOL				move;
		BOOL				capture;
		HCURSOR				cursor;
	} ClientMove;
public:
	int						m_nBkImageIndex;
	WCHAR					m_szShowText[128];

private:
	void	UpdateLayeredImageWindow(HDC hDC);

public:
	void	SetShowText(LPCWSTR lpFmt, ...);
	void	SetClientRect(int x, int y, int w, int h);
	void	SetWindowAlpha(BYTE bAlpha);
	void	GetShadeFrm(int &nLeft, int &nTop);

public:
	virtual	RECT		GetInitRect();
	virtual void		OnLayerdPrint(HDC hDC, LPRECT lpRect);

	virtual void		NotifyMouseMove(){};		// 鼠标移动消息
	virtual void		NotifyLButtonClick(){};		// 鼠标单击消息
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
