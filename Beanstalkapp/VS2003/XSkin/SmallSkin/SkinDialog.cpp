#include "StdAfx.h"
#include ".\skindialog.h"

CSkinDialog::CSkinDialog(HWND hWnd)
:CSkinWnd(hWnd)
{
}

CSkinDialog::~CSkinDialog(void)
{
}

BOOL CSkinDialog::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lRet)
{
	if(FALSE != CSkinWnd::WindowProc(Msg, wParam, lParam, lRet))
		return TRUE;

	switch (Msg)
	{
	default:
		break;
	}

	return FALSE;
}

BOOL CSkinDialog::OnNcPrint()
{
	return FALSE;
}

BOOL CSkinDialog::OnEraseBkGng(CDC *pDC)
{
	CRect		rect;

	GetClientRect(m_hWnd, &rect);
	pDC->FillSolidRect(&rect, RGB(0xff, 0xff, 0xff));
	
	return TRUE;
}