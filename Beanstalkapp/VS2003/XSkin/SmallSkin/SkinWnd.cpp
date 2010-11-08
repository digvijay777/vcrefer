#include "StdAfx.h"
#include ".\skinwnd.h"

CSkinWnd::CSkinWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

CSkinWnd::~CSkinWnd(void)
{
}

// 返回TRUE就是不再执行下面的过程
BOOL CSkinWnd::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lRet)
{
	switch(Msg)
	{
	case WM_DESTROY:
		return Destroy();
	case WM_NCPAINT:
		return OnNcActive();
	case WM_NCACTIVATE:
		return OnNcPrint();
	case WM_ERASEBKGND:
		return OnEraseBkGng(CDC::FromHandle((HDC)wParam));
	}

	return FALSE;
}

BOOL CSkinWnd::Destroy()
{
	::SetProp(m_hWnd, STR_SKINWND_PROP_PTR, NULL);
	return TRUE;
}

BOOL CSkinWnd::OnNcPrint()
{
	return FALSE;
}
BOOL CSkinWnd::OnNcActive()
{
	return FALSE;
}
BOOL CSkinWnd::OnEraseBkGng(CDC* pDC)
{
	return FALSE;
}