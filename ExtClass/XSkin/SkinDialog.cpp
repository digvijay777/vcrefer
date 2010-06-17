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

