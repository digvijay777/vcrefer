#pragma once
#include "GdipWnd.h"

class CGdipApp
{
public:
	CGdipApp();
	~CGdipApp();

private:
	HHOOK		m_hhook;
	ULONG		m_pGdiToken;

public:
	BOOL		InitGdipWndApp();
	BOOL		AttachWindow(HWND hWnd, CREATESTRUCT* pCreate);
	BOOL		AttachGdipWnd(HWND hWnd, CGdipWnd* pWnd);


	float		GetScaleX();
	float		GetScaleY();
	void		Scale(REAL* pX, REAL* pY);
	void		ScaleRect(Rect* pRect);
public:
	static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

};

extern CGdipApp		theGdipApp;