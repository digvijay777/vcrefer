#pragma once
#include <atltypes.h>
#include <comdef.h>

#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>

using namespace Gdiplus;

// GDIPWnd ¶Ô»°¿ò 
class CGdipWnd
{
public:
	CGdipWnd(HWND hWnd);
	virtual ~CGdipWnd();

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

class CGdipMemDC
{
public:
	CGdipMemDC(HDC hDC, int nWidth, int nHeight);
	virtual ~CGdipMemDC();

protected:
	HDC			m_hDC;
	HBITMAP		m_hBitmap;
	Graphics*	m_graphics;
	
public:
	HDC			GetDC();
	Graphics* operator->();
};

class CGdipCtrlDC : public CGdipMemDC
{
public:
	CGdipCtrlDC(HDC hDC, LPRECT lpRect);
	virtual ~CGdipCtrlDC();
protected:
	RECT		m_rect;
	HDC			m_hCtrlDC;
public:
	BOOL		DrawCtrl();
};
