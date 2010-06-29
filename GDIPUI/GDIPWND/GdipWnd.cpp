#include "stdafx.h"
#include "GdipWnd.h"
#include "GdipApp.h"

#pragma comment(lib, "gdiplus.lib")

//////////////////////////////////////////////////////////////////////////
// Gdip Wnd
CGdipWnd::CGdipWnd(HWND hWnd)
{
	ASSERT(IsWindow(hWnd));

	m_hWnd = hWnd;
	m_fScaleX = 1.0;
	m_fScaleY = 1.0;

	// �õ����ű���
	HDC		hDC			= GetDC(hWnd);
	int		nCurDPI		= GetDeviceCaps(hDC, LOGPIXELSX);

	if(96 != nCurDPI)
	{
		float	f96Pixel	= 0.0;
		float	fCurPixel	= 0.0;
		HFONT	hFont;

		hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
		f96Pixel = GetDPIPixel(hDC, 96, hFont);
		fCurPixel = GetDPIPixel(hDC, GetDeviceCaps(hDC, LOGPIXELSX), hFont);
		m_fScaleX = fCurPixel / f96Pixel;
		f96Pixel = GetDPIPixel(hDC, 96, hFont, TRUE);
		fCurPixel = GetDPIPixel(hDC, GetDeviceCaps(hDC, LOGPIXELSX), hFont, TRUE);
		m_fScaleY = fCurPixel / f96Pixel;
	}

	ReleaseDC(hWnd, hDC);
}

CGdipWnd::~CGdipWnd()
{

}

// �õ�DPI�ֿ�
float CGdipWnd::GetDPIPixel(HDC hDC, int nDPI, HFONT hFont, BOOL bHeight /* = FALSE */, int nWidth /* = 1 */)
{
	float		fFontWidth		= 9.0;		// ��DPI��96Ϊ����
	float		fFontHeight		= 20.0;		// ��DPI=96Ϊ����
	float		fRet			= 1.0;
	CFont*		pFont			= CFont::FromHandle(hFont);
	LOGFONT		logFont;
	CFont		font;

	pFont->GetLogFont(&logFont);
	logFont.lfHeight = (float)logFont.lfHeight * ((float)nDPI / (float)GetDeviceCaps(hDC, LOGPIXELSX));
	font.CreateFontIndirect(&logFont);

	GetFontSize(hDC, (HFONT)font.GetSafeHandle(), &fFontWidth, &fFontHeight);	
	if(FALSE == bHeight)
		fRet = fFontWidth * (float)nWidth / 4;
	else
		fRet = fFontHeight * (float)nWidth / 8;

	return fRet;
}
// �õ��ֿ�
BOOL CGdipWnd::GetFontSize(HDC hDC, HFONT hFont, float* pWidth, float* pHeight)
{
	HFONT			hOldFont		= (HFONT)::SelectObject(hDC, (HGDIOBJ)hFont);
	TEXTMETRICA		tm;
	SIZE			sz;
	static const char alphabet[] = {
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
			'r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H',
			'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',0};

	GetTextMetricsA(hDC, &tm);
	GetTextExtentPointA(hDC, alphabet, 52, &sz);

	*pHeight = tm.tmHeight;
	*pWidth = (float)(LONG)((sz.cx / 26 + 1) / 2);

	::SelectObject(hDC, (HGDIOBJ)hOldFont);
	return TRUE;
}
// ת������
void CGdipWnd::ScreenToClient(HWND hWnd, LPRECT lpRect)
{
	POINT			point;

	point.x = lpRect->left;
	point.y = lpRect->top;
	::ScreenToClient(hWnd, &point);
	lpRect->left = point.x;
	lpRect->top = point.y;
	point.x = lpRect->right;
	point.y = lpRect->bottom;
	::ScreenToClient(hWnd, &point);
	lpRect->right = point.x;
	lpRect->bottom = point.y;
}
// ����Ĭ�ϴ��ڹ���
LRESULT	CGdipWnd::DefWindowProc(IN UINT Msg, IN WPARAM wParam, IN LPARAM lParam)
{
	WNDPROC		wndProc		= (WNDPROC)GetProp(m_hWnd, GDIPAPP_WNDPROC);

	if(NULL == wndProc)
		return ::DefWindowProc(m_hWnd, Msg, wParam, lParam);

	return CallWindowProc(wndProc, m_hWnd, Msg, wParam, lParam);
}

// ���ڹ���
BOOL CGdipWnd::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return FALSE;
}