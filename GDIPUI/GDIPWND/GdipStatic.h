#pragma once
#include "GdipWnd.h"

class CGdipStatic : public CGdipWnd
{
public:
	CGdipStatic(HWND hWnd);
	~CGdipStatic();
public:
	virtual BOOL		WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT		OnPaint();
};