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

public:
	LRESULT		DefWindowProc(IN UINT Msg, IN WPARAM wParam, IN LPARAM lParam);

public:
	virtual BOOL		WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
