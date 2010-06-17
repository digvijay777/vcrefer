/*
 *	MFCGDIP 公用头文件实现部分
 */
#include "stdafx.h"
#include "MfcGdipAfx.h"

#pragma comment(lib, "gdiplus.lib")

#if(WINVER < 0x0500)
#define WS_EX_NOINHERITLAYOUT   0x00100000L // Disable inheritence of mirroring by children
#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
#endif /* WINVER >= 0x0500 */

//////////////////////////////////////////////////////////////////////////
// 函数
// 透明背景
BOOL MfcGdip::DrawTranspBkgnd(HWND hWnd, HWND hParent, Graphics* const pGraphics /* = NULL */)
{
	Graphics*	pDraw		= pGraphics;

	if(NULL == pGraphics)
		pDraw = Graphics::FromHWND(hWnd);
	if(NULL == pDraw)
		return FALSE;

	XRect		rtWnd;
	XRect		rtParent;
	HDC			hDC;

	GetWindowRect(hWnd, &rtWnd);
	MfcGdip::ScreenToClient(hParent, &rtWnd);
	GetClientRect(hParent, &rtParent);
	pDraw->TranslateTransform(REAL(rtParent.left - rtWnd.left), REAL(rtParent.top - rtWnd.top));
	hDC = pDraw->GetHDC();
	SendMessage(hParent, WM_ERASEBKGND, (WPARAM)hDC, 0);
	pDraw->ReleaseHDC(hDC);
	pDraw->ResetTransform();
	if(NULL == pGraphics)	// 清除
		delete pDraw;
	return TRUE;
}

// 从标转换
void MfcGdip::ScreenToClient(HWND hWnd, LPRECT lpRect)
{
	ASSERT(::IsWindow(hWnd));
	::ScreenToClient(hWnd, (LPPOINT)lpRect);
	::ScreenToClient(hWnd, ((LPPOINT)lpRect)+1);
	if (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		CRect::SwapLeftRight(lpRect);
}
//////////////////////////////////////////////////////////////////////////
// 配置类
CGdipConfig		gdipConfig;

CGdipConfig::CGdipConfig()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &gdiplusStartupInput, NULL);
}

CGdipConfig::~CGdipConfig()
{
	GdiplusShutdown(m_pGdiToken);
}
//////////////////////////////////////////////////////////////////////////
//
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

	rt.X = (Gdiplus::REAL)left;
	rt.Y = (Gdiplus::REAL)top;
	rt.Width = (Gdiplus::REAL)Width();
	rt.Height = (Gdiplus::REAL)Height();

	return rt;
}

void XRect::operator=(Gdiplus::Rect& rt)
{
	left = (LONG)rt.X;
	top = (LONG)rt.Y;
	right = (LONG)(rt.X + rt.Width);
	bottom = (LONG)(rt.Y + rt.Height);
}

void XRect::operator=(Gdiplus::RectF& rt)
{
	left = (LONG)rt.X;
	top = (LONG)rt.Y;
	right = (LONG)(rt.X + rt.Width);
	bottom = (LONG)(rt.Y + rt.Height);
}

//////////////////////////////////////////////////////////////////////////
//
