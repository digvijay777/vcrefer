#pragma once
#include "../mfc/MemDc.h"

#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>

#define STR_SKINWND_PROP			"SmallSkin"
#define STR_SKINWND_PROP_PTR		"SmallSkinPtr"

using namespace Gdiplus;

class XRect : public CRect
{
public:
	XRect();
	~XRect();
public:
	Gdiplus::Rect		Rect();
	Gdiplus::RectF		RectF();

	void operator=(Gdiplus::Rect& rt);
	void operator=(Gdiplus::RectF& rt);
};

//////////////////////////////////////////////////////////////////////////
class CSkinWnd
{
public:
	CSkinWnd(HWND hWnd);
	virtual ~CSkinWnd(void);

public:
	HWND		m_hWnd;
	DWORD		m_dwCapBtnState;
	BOOL		m_bTrackMouseEvent;
	DWORD		m_dwLButtonState;
public:
	virtual BOOL	WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lRet);
	virtual BOOL	Destroy();
	virtual BOOL	OnNcActive();
	virtual BOOL	OnNcPrint();
	virtual BOOL	OnEraseBkGng(CDC* pDC);
	virtual BOOL	OnCreate();
	virtual BOOL	OnNcCalSize(BOOL bResize, NCCALCSIZE_PARAMS* pSizeParam);
	virtual BOOL	OnNcHitTest(CPoint point, int* pHit);
	virtual BOOL	OnNcMouseMove(int nHit, CPoint point);
	virtual BOOL	OnNcMouseLeave();
	virtual BOOL	OnNcLButtonDown(int nHit, CPoint point);
	virtual BOOL	OnLButtonUp(int nState, CPoint point);
	virtual BOOL	OnMouseMove(int nState, CPoint point);
	virtual BOOL	OnDrawItem(UINT idCtrl, LPDRAWITEMSTRUCT lpdis);

	virtual void	DrawWindowFrame(Graphics& graphics, XRect rect);
	virtual void	DrawTitleText(Graphics& graphics, XRect rect);
	virtual void	DrawTitleSysButton(Graphics& graphics, XRect rtTitle, int nMinState = -1, int nMaxState = -1, int nClose = -1);
protected:
	void		CreateRgnWnd();

	void		ReDrawSysButton();
	XRect		CalCaptionText(XRect rtCaption);
	XRect		CalCaptionIcon(XRect rtCaption);
	XRect		CalCaptionSysCloseBox(XRect rtCaption);
	XRect		CalCaptionSysMaxBox(XRect rtCaption);
	XRect		CalCaptionSysMinBox(XRect rtCaption);
};
