#pragma once
#include "GdipWnd.h"

#define GDIPAPP_WNDPROC			_T("_GdipWnd_WNDPROC_")
#define GDIPAPP_WNDPTR			_T("_GdipWnd_WndPtr_")

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
	BOOL		DetachWindow(HWND hWnd);
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