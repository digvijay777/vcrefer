#pragma once
#include "..\..\..\ExtClass\xskin\skindialog.h"

class CLunaDialog :
	public CSkinDialog
{
public:
	CLunaDialog(HWND hWnd);
	virtual ~CLunaDialog(void);

public:
	virtual BOOL	OnEraseBkGng(CDC* pDC);
};
