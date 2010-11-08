#include "StdAfx.h"
#include ".\lunaapp.h"
#include "LunaDialog.h"

CLunaApp::CLunaApp(void)
{
}

CLunaApp::~CLunaApp(void)
{
}

BOOL CLunaApp::AttachWindow(HWND hWnd, CREATESTRUCT* pCreate)
{
	CHAR		szClassName[MAX_PATH]		= {0};
	HANDLE		hWndProc					= GetOldWndProc(hWnd);

	if(NULL != hWndProc)
		return TRUE;

	GetClassName(hWnd, szClassName, sizeof(szClassName));
	if(stricmp("#32770", szClassName) == 0)
	{
		return AttachSkinWindow(hWnd, new CLunaDialog(hWnd));
	}
	else 
	{
		return CSkinApp::AttachWindow(hWnd, pCreate);
	}

	return FALSE;
}