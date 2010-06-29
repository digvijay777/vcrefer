#pragma once
#include <atltypes.h>

#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>

using namespace Gdiplus;

// GDIPWnd ¶Ô»°¿ò 
class CGdipWnd
{
public:
	CGdipWnd(HWND hWnd);
	~CGdipWnd();

public:
	HWND	m_hWnd;
	float	m_fScaleX;
	float	m_fScaleY;

public:
	float	GetDPIPixel(HDC hDC, int nDPI, HFONT hFont, BOOL bHeight = FALSE, int nWidth = 1);
	BOOL	GetFontSize(HDC hDC, HFONT hFont, float* pWidth, float* pHeight);
	void	ScreenToClient(HWND hWnd, LPRECT lpRect);
public:
	LRESULT		DefWindowProc(IN UINT Msg, IN WPARAM wParam, IN LPARAM lParam);

public:
	virtual BOOL		WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
