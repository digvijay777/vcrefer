#include "StdAfx.h"
#include ".\lunadialog.h"

CLunaDialog::CLunaDialog(HWND hWnd)
:CSkinDialog(hWnd)
{
}

CLunaDialog::~CLunaDialog(void)
{
}

// ���ƴ��屳��
BOOL CLunaDialog::OnEraseBkGng(CDC* pDC)
{
	return FALSE;
}
