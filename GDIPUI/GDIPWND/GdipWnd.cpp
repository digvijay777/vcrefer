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