#pragma once
#include "skinwnd.h"

class CSkinDialog :
	public CSkinWnd
{
public:
	CSkinDialog(HWND hWnd);
	virtual ~CSkinDialog(void);
public:
	virtual BOOL	OnNcPrint();
	virtual BOOL	WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lRet);
	virtual BOOL	OnEraseBkGng(CDC *pDC);
};
