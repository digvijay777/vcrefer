#include "StdAfx.h"
#include ".\skinwnd.h"
#include "SkinApp.h"

#pragma comment(lib, "gdiplus.lib")

#define SKINWND_FRAME_BORDER		2
#define SKINWND_CAPTION				24
#define SKINWND_CAPTION_SYSBTN		5
#define SKINWND_CAPTION_LEFT		5
#define SKINWND_CAPTION_SYSBTN_W	18
#define SKINWND_CAPTION_SYSBTN_H	16
#define SKINWND_CAPTION_SYSBTN_T	4

CSkinWnd::CSkinWnd(HWND hWnd)
{
	m_hWnd = hWnd;
	m_dwCapBtnState = 0;
	m_bTrackMouseEvent = FALSE;
	m_dwLButtonState = 0;
}

CSkinWnd::~CSkinWnd(void)
{
}

// 返回TRUE就是不再执行下面的过程
BOOL CSkinWnd::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lRet)
{
	lRet = 0;

	switch(Msg)
	{
	case WM_DESTROY:
		return Destroy();
	case WM_NCPAINT:
		lRet = -1;
		return OnNcActive();
	case WM_NCACTIVATE:
		lRet = -1;
		return OnNcPrint();
	case WM_ERASEBKGND:
		return OnEraseBkGng(CDC::FromHandle((HDC)wParam));
	case WM_CREATE:
		return OnCreate();
	case WM_NCCALCSIZE:
		return OnNcCalSize((BOOL)wParam, (NCCALCSIZE_PARAMS *)lParam);
	case WM_NCHITTEST:
		return OnNcHitTest(CPoint(LOWORD(lParam), HIWORD(lParam)), (int *)&lRet);
	case WM_MOUSEMOVE:
		return OnMouseMove((int)wParam, CPoint(LOWORD(lParam), HIWORD(lParam)));
	case WM_NCMOUSEMOVE:
		return OnNcMouseMove((int)wParam, CPoint(LOWORD(lParam), HIWORD(lParam)));
	case WM_NCMOUSELEAVE:
		return OnNcMouseLeave();
	case WM_NCLBUTTONDOWN:
		return OnNcLButtonDown((int)wParam, CPoint(LOWORD(lParam), HIWORD(lParam)));
	case WM_NCLBUTTONUP:
	case WM_LBUTTONUP:
		return OnLButtonUp((int)wParam, CPoint(LOWORD(lParam), HIWORD(lParam)));
	case WM_SETTEXT:
		return OnSetText((LPSTR)wParam);
	case WM_DRAWITEM:
		return 3;
	case WM_ENTERIDLE:
		return OnEnterIdle(wParam, lParam);
	case WM_INITMENU:
		{
			HMENU		hMenu		 = (HMENU)wParam;

			
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CSkinWnd::Destroy()
{
	::SetProp(m_hWnd, STR_SKINWND_PROP_PTR, NULL);
	return TRUE;
}

BOOL CSkinWnd::OnNcPrint()
{
	CDC*			pDC			= CDC::FromHandle(GetWindowDC(m_hWnd));
	DWORD			dwStyle		= GetWindowLong(m_hWnd, GWL_STYLE);
	XRect			rtClient;
	XRect			rtCaption;
	Graphics		graphics(pDC->GetSafeHdc());

	GetWindowRect(m_hWnd, &rtClient);
	
	// 绘制边框
	if(WS_BORDER == (WS_BORDER & dwStyle))
	{
		DrawWindowFrame(graphics, rtClient);
		rtCaption.DeflateRect(SKINWND_FRAME_BORDER, SKINWND_FRAME_BORDER, SKINWND_FRAME_BORDER, 0);
	}

	if(WS_CAPTION != (WS_CAPTION & dwStyle))
		return TRUE;

	// 绘制标题
	CDC				memDC;
	CBitmap			bitmap;
	CBitmap*		pOldBmp		= NULL;

	rtCaption = rtClient;
	rtCaption.MoveToXY(2, 2);
	rtCaption.right -= 4;
	rtCaption.bottom = rtCaption.top + SKINWND_CAPTION;
	if(FALSE == memDC.CreateCompatibleDC(pDC))
		return FALSE;
	if(FALSE == bitmap.CreateCompatibleBitmap(pDC, rtCaption.Width(), rtCaption.Height()))
		return FALSE;
	pOldBmp = memDC.SelectObject(&bitmap);
	
	Graphics		graphMem(memDC.GetSafeHdc());
	XRect			rtTitle			= rtCaption;

	rtTitle.MoveToXY(0, 0);
	DrawTitleText(graphMem, rtTitle);

	// 绘制系统按钮
	int				nMinState = -1;
	int				nMaxState = -1;
	int				nCloseState = 0xf & m_dwCapBtnState;

	if(WS_MAXIMIZEBOX == (WS_MAXIMIZEBOX & dwStyle))
		nMaxState = (0xf0 & m_dwCapBtnState) >> 4;
	if(WS_MINIMIZEBOX == (WS_MINIMIZEBOX & dwStyle))
		nMinState = (0xf00 & m_dwCapBtnState) >> 8;
	DrawTitleSysButton(graphMem, rtTitle, nMinState, nMaxState, nCloseState);

	// 完成绘制
	pDC->BitBlt(rtCaption.left, rtCaption.top, rtCaption.Width(), rtCaption.Height()
		, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);

	ReleaseDC(m_hWnd, pDC->m_hDC);

	return TRUE;
}
BOOL CSkinWnd::OnNcActive()
{
	return OnNcPrint();
}

BOOL CSkinWnd::OnEraseBkGng(CDC* pDC)
{
	return FALSE;
}

BOOL CSkinWnd::OnCreate()
{
	CreateRgnWnd();

	return FALSE;
}
BOOL CSkinWnd::OnNcCalSize(BOOL bResize, NCCALCSIZE_PARAMS* pSizeParam)
{
	int		nBorder		= SKINWND_FRAME_BORDER;
	int		nCaption	= SKINWND_CAPTION;
	DWORD	dwStyle		= GetWindowLong(m_hWnd, GWL_STYLE);

	if(WS_BORDER != (WS_BORDER & dwStyle))
		nBorder = 0;
	if(WS_CAPTION != (WS_CAPTION & dwStyle))
		nCaption = 0;

	if(FALSE != bResize)
	{
		nBorder = pSizeParam->rgrc[2].left - pSizeParam->rgrc[1].left;
		nCaption = pSizeParam->rgrc[2].top - pSizeParam->rgrc[1].top - nBorder;
	}

	// pSizeParam 初如指向窗体 
	pSizeParam->rgrc[0].top += (nCaption+nBorder);  
	pSizeParam->rgrc[0].bottom -= nBorder;   
	pSizeParam->rgrc[0].left += nBorder;   
	pSizeParam->rgrc[0].right -= nBorder;  

	// 菜单的大小
// 	HMENU		hMenu		= GetMenu(m_hWnd);
// 
// 	if(NULL != hMenu)
// 	{
// 		MENUINFO		info	= {0};
// 
// 		info.cbSize = sizeof(info);
// 		::GetMenuInfo(hMenu, &info);
// 		
// 		
// 		pSizeParam->rgrc[0].top += 20;//GetSystemMetrics(SM_CYMENUSIZE);
// 	}
	return TRUE;
}

void CSkinWnd::CreateRgnWnd()
{

}

// 绘制边框
void CSkinWnd::DrawWindowFrame(Graphics& graphics, XRect rect)
{
	Pen				pFrame1(Color(0xff, 0x5b, 0x8f, 0xee));
	Pen				pFrame2(Color(0xff, 0x9D, 0xcA, 0xf5));

	rect.MoveToXY(0, 0);
	rect.right -= 1;
	rect.bottom -= 1;
	graphics.DrawRectangle(&pFrame1, rect.Rect());
	rect.DeflateRect(1, 1, 1, 1);
	graphics.DrawRectangle(&pFrame2, rect.Rect());
}

// 绘制标题栏
void CSkinWnd::DrawTitleText(Graphics& graphics, XRect rect)
{
	LinearGradientBrush		brush(rect.Rect()
		, Color(0xff, 0x96, 0xd3, 0xf7)
		, Color(0xff, 0x40, 0x5d, 0xa0)
		, LinearGradientModeVertical);
	Color	colors[]	= {Color(0xff, 0x96, 0xd3, 0xf7)
		, Color(0xff, 0x84, 0xbb, 0xe5)
		, Color(0xff, 0x64, 0x8e, 0xc4)
		, Color(0xff, 0x40, 0x5d, 0xa0)};
	REAL	positions[] = {0.0f		// 超始
		, 0.45f	// 画刷长度的2/3
		, 0.45f	
		, 1.0f	// 终止
	};
	// 标题栏背景
//	brush.TranslateTransform(0, SKINWND_CAPTION / 2);
	brush.SetInterpolationColors(colors, positions, 4);
	graphics.FillRectangle(&brush, rect.Rect());

	// 文字
	CStringW			strTitle;
	FontFamily	fontFamily(L"宋体");
	Font			font(&fontFamily, 12, FontStyleRegular, UnitPixel);
	StringFormat		format;
	XRect				rtText		= CalCaptionText(rect);

	::GetWindowTextW(m_hWnd, strTitle.GetBufferSetLength(1024), 1024);
	strTitle.ReleaseBuffer();
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetFormatFlags(StringFormatFlagsLineLimit);
	format.SetTrimming(StringTrimmingEllipsisCharacter);

	graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
	graphics.DrawString(strTitle.GetBuffer(),  -1, &font
		, rtText.RectF()
		, &format
		, &SolidBrush(Color(255, 0xff, 0xff, 0xff)));

	// 绘制图标
	HICON		hIcon		= (HICON)::SendMessage(m_hWnd, WM_GETICON, FALSE, 0);
	XRect		rtIcon		= CalCaptionIcon(rect);
	if(hIcon)
	{
		HDC			hDC			= graphics.GetHDC();
		::DrawIconEx(hDC, rtIcon.left, rtIcon.top, hIcon, 16, 16, 0, NULL, DI_NORMAL);
		graphics.ReleaseHDC(hDC);
	}
}
// 绘制系统菜单
void CSkinWnd::DrawTitleSysButton(Graphics& graphics, XRect rtTitle, int nMinState /* = -1 */, 
								int nMaxState /* = -1 */, int nClose /* = -1 */)
{
	XRect		rtClose		= CalCaptionSysCloseBox(rtTitle);
	GraphicsPath					path;
	int								nArc		= 4;
	int								nWidth		= SKINWND_CAPTION_SYSBTN_W;
	int								nHeight		= SKINWND_CAPTION_SYSBTN_H;

	// 构建矩形
	path.AddArc(nWidth - nArc, 0, nArc, nArc, 270, 90);
	path.AddArc(nWidth - nArc, nHeight - nArc, nArc, nArc, 0, 90);
	path.AddArc(0, nHeight - nArc, nArc, nArc, 90, 90);
	path.AddArc(0, 0, nArc, nArc, 180, 90);
	path.AddLine(nArc, 0, nWidth - nArc / 2, 0);

	// 构建颜色
	Color		colors[]	= {Color(0xff, 0x92, 0xb4, 0xf4)
		, Color(0xff, 0x5e, 0x91, 0xee)
		, Color(0xff, 0x92, 0xb4, 0xf4)
	};
	Color		colors2[]	= {Color(0xff, 0xdf, 0x96, 0x80)
		, Color(0xff, 0xd3, 0x6e, 0x4f)
		, Color(0xff, 0xdf, 0x96, 0x80)
	};
	REAL			postions[]	= {0.0f
		, 0.5f
		, 1.0f
	};
	LinearGradientBrush		bhNormal(rtClose.Rect()
		, Color(0xff, 0x91, 0xc7, 0xf0)
		, Color(0xff, 0x53, 0x70, 0x0)
		, LinearGradientModeVertical);
	LinearGradientBrush		bhHover(rtClose.Rect()
		, Color(0xff, 0xd3, 0x6e, 0x4f)
		, Color(0xff, 0xdf, 0x96, 0x80)
		, LinearGradientModeVertical);
	LinearGradientBrush		bhDown(rtClose.Rect()
		, Color(0xff, 0xd3, 0x6e, 0x4f)
		, Color(0xff, 0xdf, 0x96, 0x80)
		, LinearGradientModeVertical);
	Pen						phHover(Color(0xcc, 0xd0, 0x6e, 0x51));
	Pen						phNormal(Color(0xcc, 0x5e, 0x91, 0xee));
	Pen						pLine(Color(0xcc, 0xff, 0xff, 0xff), 1);
	Pen						pLine2(Color(0xcc, 0xff, 0xff, 0xff), 2);
	Point					ptClose[]	= {
		Point(nWidth / 3, nHeight / 3), Point(nWidth - nWidth / 3, nHeight - nHeight / 3)
		, Point(nWidth - nWidth / 3, nHeight / 3), Point(nWidth / 3, nHeight - nHeight / 3)
	};

	bhNormal.SetInterpolationColors(colors, postions, 3);
	phNormal.SetAlignment(PenAlignmentInset);
	bhHover.SetInterpolationColors(colors2, postions, 3);
	phHover.SetAlignment(PenAlignmentInset);

 	if(-1 != nClose)
	{
		graphics.TranslateTransform(rtClose.left, rtClose.top);
		switch (nClose)
		{
		case 2:
			graphics.FillPath(&bhDown, &path);
			graphics.DrawPath(&phHover, &path);
			break;
		case 1:
			graphics.FillPath(&bhHover, &path);
			graphics.DrawPath(&phHover, &path);
			break;
		default:
			graphics.FillPath(&bhNormal, &path);
			graphics.DrawPath(&phNormal, &path);
			break;
		}
		graphics.DrawLine(&pLine2, ptClose[0], ptClose[1]);
		graphics.DrawLine(&pLine2, ptClose[2], ptClose[3]);
		graphics.ResetTransform();
	}
	if(-1 != nMaxState)
	{
		XRect		rtMax		= CalCaptionSysMaxBox(rtTitle);
		graphics.TranslateTransform(rtMax.left, rtMax.top);
		switch (nMaxState)
		{
		case 2:
			graphics.FillPath(&bhDown, &path);
			graphics.DrawPath(&phHover, &path);
			break;
		case 1:
			graphics.FillPath(&bhHover, &path);
			graphics.DrawPath(&phHover, &path);
			break;
		default:
			graphics.FillPath(&bhNormal, &path);
			graphics.DrawPath(&phNormal, &path);
			break;
		}
		int			n		= min(nWidth / 4, nHeight / 4);
		Point		ptMax1[]	= {Point(n, -1 * n)
			, Point(0, -1 * n)
			, Point(n, 0)
			, Point(n, -1 * n)
		};

		graphics.TranslateTransform(nWidth / 2, nHeight / 2);
		graphics.DrawLines(&pLine2, ptMax1, 4);
		graphics.RotateTransform(180);
		graphics.DrawLines(&pLine2, ptMax1, 4);
		graphics.ResetTransform();
	}
	if(-1 != nMinState)
	{
		XRect		rtMin		= CalCaptionSysMinBox(rtTitle);
		graphics.TranslateTransform(rtMin.left, rtMin.top);
		switch (nMinState)
		{
		case 2:
			graphics.FillPath(&bhDown, &path);
			graphics.DrawPath(&phHover, &path);
			break;
		case 1:
			graphics.FillPath(&bhHover, &path);
			graphics.DrawPath(&phHover, &path);
			break;
		default:
			graphics.FillPath(&bhNormal, &path);
			graphics.DrawPath(&phNormal, &path);
			break;
		}
		graphics.DrawLine(&pLine2, Point(nWidth / 3, nHeight - nHeight / 3)
			, Point(nWidth - nWidth / 3, nHeight - nHeight / 3));
		graphics.ResetTransform();
	}
}
// 得到标题栏的区域
XRect CSkinWnd::CalCaptionIcon(XRect rtCaption)
{
	XRect		rtIcon;

	rtIcon.left = rtCaption.left + SKINWND_CAPTION_LEFT;
	rtIcon.top = rtCaption.top + (rtCaption.Height() - 16) / 2;
	rtIcon.right = rtIcon.left + 16;
	rtIcon.bottom = rtIcon.top + 16;

	return rtIcon;
}
XRect CSkinWnd::CalCaptionText(XRect rtCaption)
{
	XRect		rtText;
	int			nRight		= 1 + SKINWND_CAPTION_SYSBTN_W + SKINWND_CAPTION_SYSBTN;
	int			nLeft		= SKINWND_CAPTION_LEFT;
	DWORD		dwStyle		= GetWindowLong(m_hWnd, GWL_STYLE);

	if(NULL != ::SendMessage(m_hWnd, WM_GETICON, FALSE, 0))
		nLeft += 16 + 1;
	if(WS_MAXIMIZEBOX == (WS_MAXIMIZEBOX & dwStyle))
		nRight += SKINWND_CAPTION_SYSBTN_W;
	if(WS_MINIMIZEBOX == (WS_MINIMIZEBOX & dwStyle))
		nRight += SKINWND_CAPTION_SYSBTN_W;

	rtText.left = rtCaption.left + nLeft;
	rtText.top = rtCaption.top;
	rtText.bottom = rtCaption.bottom;
	rtText.right = rtCaption.right - nLeft - nRight;

	return rtText;
}
XRect CSkinWnd::CalCaptionSysCloseBox(XRect rtCaption)
{
	XRect		rtClose;
	
	rtClose.left = rtCaption.right - (SKINWND_CAPTION_SYSBTN_W) - SKINWND_CAPTION_SYSBTN;
	rtClose.top = rtCaption.top + SKINWND_CAPTION_SYSBTN_T;
	rtClose.right = rtClose.left + SKINWND_CAPTION_SYSBTN_W;
	rtClose.bottom = rtClose.top + SKINWND_CAPTION_SYSBTN_H;

	return rtClose;
}

XRect CSkinWnd::CalCaptionSysMaxBox(XRect rtCaption)
{
	XRect		rtMax;

	rtMax.left = rtCaption.right - 2 * (SKINWND_CAPTION_SYSBTN_W) - SKINWND_CAPTION_SYSBTN;
	rtMax.top = rtCaption.top + SKINWND_CAPTION_SYSBTN_T;
	rtMax.right = rtMax.left + SKINWND_CAPTION_SYSBTN_W;
	rtMax.bottom = rtMax.top + SKINWND_CAPTION_SYSBTN_H;

	return rtMax;
}

XRect CSkinWnd::CalCaptionSysMinBox(XRect rtCaption)
{
	XRect		rtMin;
	DWORD		dwStyle		= GetWindowLong(m_hWnd, GWL_STYLE);

	rtMin.left = rtCaption.right - 2 * (SKINWND_CAPTION_SYSBTN_W) - SKINWND_CAPTION_SYSBTN;
	rtMin.top = rtCaption.top + SKINWND_CAPTION_SYSBTN_T;
	if(WS_MAXIMIZEBOX == (WS_MAXIMIZEBOX & dwStyle))
		rtMin.left -= SKINWND_CAPTION_SYSBTN_W;
	rtMin.right = rtMin.left + SKINWND_CAPTION_SYSBTN_W;
	rtMin.bottom = rtMin.top + SKINWND_CAPTION_SYSBTN_H;

	return rtMin;
}
// 测试区域
BOOL CSkinWnd::OnNcHitTest(CPoint point, int* pHit)
{
	XRect		rect;
	DWORD		dwStyle		= 0;
	
	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if(WS_CAPTION != (WS_CAPTION & dwStyle))
		return FALSE;

	GetWindowRect(m_hWnd, &rect);
	rect.DeflateRect(2, 2, 2, 0);
	rect.bottom = rect.top + SKINWND_CAPTION;
	if(!rect.PtInRect(point))
		return FALSE;
	else if(point.y > rect.bottom)
	{
		*pHit = HTMENU;
		return TRUE;
	}
    
	*pHit = 0;

	int		nRight		= SKINWND_CAPTION_SYSBTN_W + SKINWND_CAPTION_SYSBTN;
	if(WS_MAXIMIZEBOX == (WS_MAXIMIZEBOX & dwStyle))
		nRight += SKINWND_CAPTION_SYSBTN_W + SKINWND_CAPTION_SYSBTN;
	if(WS_MINIMIZEBOX == (WS_MINIMIZEBOX & dwStyle))
		nRight += SKINWND_CAPTION_SYSBTN_W + SKINWND_CAPTION_SYSBTN;
	XRect	rtCaption	= rect;

    if(point.x <= (rect.right - nRight))
	{
		*pHit = HTCAPTION;
	}
	else if(CalCaptionSysCloseBox(rtCaption).PtInRect(point))
	{
		*pHit = HTCLOSE;
	}
	else if((WS_MAXIMIZEBOX == (WS_MAXIMIZEBOX & dwStyle)) 
		&& CalCaptionSysMaxBox(rtCaption).PtInRect(point))
	{
		*pHit = HTMAXBUTTON;
	}
	else if((WS_MINIMIZEBOX == (WS_MINIMIZEBOX & dwStyle))
		&& CalCaptionSysMinBox(rtCaption).PtInRect(point))
	{
		*pHit = HTMINBUTTON;
	}

	return TRUE;
}
BOOL CSkinWnd::OnNcMouseLeave()
{
	if(0 == m_dwLButtonState)
	{
		m_dwCapBtnState = 0;
		ReDrawSysButton();
	}

	m_bTrackMouseEvent = FALSE;
	return TRUE;
}
// 鼠标移动
BOOL CSkinWnd::OnNcMouseMove(int nHit, CPoint point)
{
	if(FALSE == m_bTrackMouseEvent)
	{
		TRACKMOUSEEVENT			tmEvent;

		tmEvent.cbSize = sizeof(tmEvent);
		tmEvent.dwHoverTime = HOVER_DEFAULT;
		tmEvent.hwndTrack = m_hWnd;
		tmEvent.dwFlags = TME_LEAVE | TME_NONCLIENT;
		m_bTrackMouseEvent = TrackMouseEvent(&tmEvent);
	}

	if(HTCLOSE != nHit 
		&& HTMAXBUTTON != nHit
		&& HTMINBUTTON != nHit
		&& (0 == m_dwCapBtnState))
		return FALSE;
	
	m_dwCapBtnState = 0;
    if(HTCLOSE == nHit)
	{
		if(1 == m_dwLButtonState)
			m_dwCapBtnState = 0x2;
		else if(0 == m_dwLButtonState)
			m_dwCapBtnState = 0x1;
	}
	else if(HTMAXBUTTON == nHit)
	{
		if(2 == m_dwLButtonState)
			m_dwCapBtnState = 0x2 << 4;
		else if(0 == m_dwLButtonState)
			m_dwCapBtnState = 0x1 << 4;
	}
	else if(HTMINBUTTON == nHit)
	{
		if(3 == m_dwLButtonState)
			m_dwCapBtnState = 0x2 << 8;
		else if(0 == m_dwLButtonState)
			m_dwCapBtnState = 0x1 << 8;
	}

	ReDrawSysButton();
	return FALSE;
}
// 鼠标按下
BOOL CSkinWnd::OnNcLButtonDown(int nHit, CPoint point)
{
	if(HTCLOSE != nHit 
		&& HTMAXBUTTON != nHit
		&& HTMINBUTTON != nHit)
		return FALSE;

	SetCapture(m_hWnd);
	if(HTCLOSE == nHit)
	{
		m_dwCapBtnState = 0x2;
		m_dwLButtonState = 1;
	}
	else if(HTMAXBUTTON == nHit)
	{
		m_dwCapBtnState = 0x2 << 4;
		m_dwLButtonState = 2;
	}
	else if(HTMINBUTTON == nHit)
	{
		m_dwCapBtnState = 0x2 << 8;
		m_dwLButtonState = 3;
	}

	ReDrawSysButton();
	return TRUE;
}
BOOL CSkinWnd::OnLButtonUp(int nState, CPoint point)
{
	if(0 != m_dwLButtonState)
	{
		ReleaseCapture();
		m_dwLButtonState = 0;
		m_dwCapBtnState = 0;
		ReDrawSysButton();
		GetCursorPos(&point);
		int		nHit	= SendMessageA(m_hWnd, WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y));

		switch(nHit)
		{
		case HTCLOSE:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		case HTMAXBUTTON:
			{
				WINDOWPLACEMENT		wp	= {0};

				GetWindowPlacement(m_hWnd, &wp);
				if(SW_SHOWMAXIMIZED == wp.showCmd)
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				else 
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
			break;
		case HTMINBUTTON:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		}
		return TRUE;
	}

	return FALSE;
}
BOOL CSkinWnd::OnMouseMove(int nState, CPoint point)
{
	if(0 != m_dwLButtonState)
	{
		GetCursorPos(&point);
		OnNcMouseMove(SendMessageA(m_hWnd, WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y)), point);
		return TRUE;
	}
	return FALSE;
}
BOOL CSkinWnd::OnSetText(LPCSTR lpText)
{
	::CallWindowProc(CSkinApp::s_pSkinApp->m_mapWnds[m_hWnd].wndProc
		, m_hWnd
		, WM_SETTEXT
		, 0, (LPARAM)lpText);

	PostMessage(m_hWnd, WM_NCACTIVATE, FALSE, 0);
	return FALSE;
}
BOOL CSkinWnd::OnEnterIdle(WPARAM wParam, LPARAM lParam)
{
	if(MSGF_MENU != wParam)
		return FALSE;

	HMENU		hMenu		 = (HMENU)lParam;

	return FALSE;
}

void CSkinWnd::ReDrawSysButton()
{
	OnNcPrint();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
XRect::XRect()
{

}

XRect::~XRect()
{

}

Gdiplus::Rect XRect::Rect()
{
	Gdiplus::Rect		rt;

	rt.X = left;
	rt.Y = top;
	rt.Width = Width();
	rt.Height = Height();

	return rt;
}

Gdiplus::RectF XRect::RectF()
{
	Gdiplus::RectF		rt;

	rt.X = left;
	rt.Y = top;
	rt.Width = Width();
	rt.Height = Height();

	return rt;
}

void XRect::operator=(Gdiplus::Rect& rt)
{
	left = rt.X;
	top = rt.Y;
	right = rt.X + rt.Width;
	bottom = rt.Y + rt.Height;
}

void XRect::operator=(Gdiplus::RectF& rt)
{
	left = rt.X;
	top = rt.Y;
	right = rt.X + rt.Width;
	bottom = rt.Y + rt.Height;
}
