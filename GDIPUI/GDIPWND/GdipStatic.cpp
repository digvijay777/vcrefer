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
	if(FALSE != CGdipWnd::WindowProc(Msg, wParam, lParam, pResult))
		return TRUE;


	switch(Msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT		paint		= {0};
			HDC				hDC			= BeginPaint(m_hWnd, &paint);

			*pResult = OnPaint(hDC);
			EndPaint(m_hWnd, &paint);
		}
		return TRUE;
	case WM_SETTEXT:
		m_strText = (LPCSTR)lParam;
		InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case WM_GETTEXT:
		if(m_strText.length() == 0)
			return FALSE;
		lstrcpynA((LPSTR)lParam, m_strText.c_str(), (int)wParam);
		*pResult = m_strText.size();
		return TRUE;
	}
	return FALSE;
}

// 绘制背景
LRESULT CGdipStatic::OnPaint(HDC hDC)
{
	HWND			hParentWnd		= GetParent(m_hWnd);
	RECT			rect;
	CHAR			szText[MAX_PATH];
	_bstr_t			bstrText;

	GetWindowRect(m_hWnd, &rect);
	ScreenToClient(hParentWnd, &rect);
	CGdipCtrlDC		memDC(hDC, &rect);
	SendMessage(hParentWnd, WM_ERASEBKGND, (WPARAM)memDC.GetDC(), 0);
	GetWindowText(m_hWnd, szText, sizeof(szText));
	bstrText = szText;
	memDC->DrawString((WCHAR*)bstrText, bstrText.length()
		, new Gdiplus::Font(L"宋体", 12.0), PointF(0.0, 0.0), new SolidBrush(Color(0xff, 0xff, 0x0, 0x0)));

	return 0;
}