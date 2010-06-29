#include "stdafx.h"
#include "GdipStatic.h"

CGdipStatic::CGdipStatic(HWND hWnd)
:CGdipWnd(hWnd)
{

}

CGdipStatic::~CGdipStatic()
{

}

// 消息过程
BOOL CGdipStatic::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch(Msg)
	{
	case WM_PAINT:
		*pResult = OnPaint();
		return TRUE;
	}
	return FALSE;
}

// 绘制背景
LRESULT CGdipStatic::OnPaint()
{
	HDC				hDC				= GetDC(m_hWnd);
	Graphics		graphics(hDC);
	RECT			rect;
	HDC				hTransDC;
	HWND			hParentWnd		= GetParent(m_hWnd);

	GetWindowRect(m_hWnd, &rect);
	ScreenToClient(hParentWnd, &rect);
	graphics.TranslateTransform(rect.left, rect.top);
	hTransDC = graphics.GetHDC();
	SendMessage(hParentWnd, WM_ERASEBKGND, (WPARAM)hDC, 0);
	graphics.ReleaseHDC(hTransDC);
	graphics.ResetTransform();

	hTransDC = graphics.GetHDC();
	DefWindowProc(WM_PAINT, (WPARAM)0, 0);
	graphics.ReleaseHDC(hTransDC);

	ReleaseDC(m_hWnd, hDC);
	return 0;
}