#pragma once
#include "..\..\..\ExtClass\XSkin\SkinApp.h"

class CLunaApp :
	public CSkinApp
{
public:
	CLunaApp(void);
	virtual ~CLunaApp(void);
public:
	virtual BOOL	AttachWindow(HWND hWnd, CREATESTRUCT* pCreate);

};
