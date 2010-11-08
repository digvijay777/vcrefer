#include "StdAfx.h"
#include ".\lunadialog.h"

CLunaDialog::CLunaDialog(HWND hWnd)
:CSkinDialog(hWnd)
{
}

CLunaDialog::~CLunaDialog(void)
{
}

// ªÊ÷∆¥∞ÃÂ±≥æ∞
BOOL CLunaDialog::OnEraseBkGng(CDC* pDC)
{
	return FALSE;
}
