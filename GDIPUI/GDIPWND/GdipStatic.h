#pragma once
#include "GdipWnd.h"
#include <string>

class CGdipStatic : public CGdipWnd
{
public:
	CGdipStatic(HWND hWnd);
	virtual ~CGdipStatic();

protected:
	std::string			m_strText;
public:
	virtual BOOL		WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT		OnPaint(HDC hDC);
};