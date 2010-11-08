#pragma once
#include "XCaptionControl.h"

class CXSkinWnd
{
protected:	// 不永许定义对象
	CXSkinWnd(CWnd*	pWnd, UINT uidCaption = 0);
	~CXSkinWnd();
private:
	CWnd*		m_pSkinWnd;
	UINT		m_uidCaption;		// 标题栏ID
	BOOL		m_bReDarw;
	BOOL		m_bNcLButtonDown;
	int			m_nTitleHeight;

protected:
	BOOL SkinWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plRet);
private:
	BOOL PtInCaption(const RECT* rect, POINT pt);
	BOOL PtInClose(const RECT* rect, POINT pt);
	void SetWindowCirRgn();
protected:
	// 非客户区消息
	virtual BOOL OnSkinCreateWnd(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL); 
	virtual BOOL OnSkinNccalSize(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
	virtual BOOL OnSkinNcPaint(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
	virtual BOOL OnSkinNcLButtonDown(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
	virtual BOOL OnSkinNcHitTest(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
	virtual BOOL OnSkinNcLbuttonUp(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
	virtual BOOL OnSkinNcRbuttonUp(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
	virtual BOOL OnSkinNcMouseMove(WPARAM wParam, LPARAM lParam, LRESULT* lpResult = NULL);
protected:
	// 绘制非客户区
	virtual void DrawWndFrame(CDC* pDC, RECT* pRect);
	virtual void DrawWndTitleBarBk(CDC* pDC, RECT* pRect);

};

// 在WindowPorc函数里调用
#define CallXSkinWndProc(msg, wParam, lParam)\
	LRESULT __result_XSkinWnd;\
	if(TRUE == CXSkinWnd::SkinWindowProc(msg, wParam, lParam, &__result_XSkinWnd))\
	{\
		return __result_XSkinWnd;\
	}\


