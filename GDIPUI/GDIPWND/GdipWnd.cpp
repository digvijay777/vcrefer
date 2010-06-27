#include "stdafx.h"
#include "GdipWnd.h"
#include "GdipApp.h"

#pragma comment(lib, "gdiplus.lib")

//////////////////////////////////////////////////////////////////////////
// Gdip Wnd
CGdipWnd::CGdipWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

CGdipWnd::~CGdipWnd()
{

}

// 调用默认窗口过程
LRESULT	CGdipWnd::DefWindowProc(IN UINT Msg, IN WPARAM wParam, IN LPARAM lParam)
{
	WNDPROC		wndProc		= (WNDPROC)GetProp(m_hWnd, GDIPAPP_WNDPROC);

	if(NULL == wndProc)
		return ::DefWindowProc(m_hWnd, Msg, wParam, lParam);

	return CallWindowProc(wndProc, m_hWnd, Msg, wParam, lParam);
}

// 窗口过程
BOOL CGdipWnd::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return FALSE;
}