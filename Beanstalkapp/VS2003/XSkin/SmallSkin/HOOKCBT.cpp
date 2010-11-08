#include "StdAfx.h"
#include "HOOKCBT.h"
#include "SkinDialog.h"

HHOOK				ghookCBT			= NULL;

LRESULT CALLBACK HOOKCBT::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch(nCode)
	{
	case HCBT_CREATEWND:
		HOOKCBT::AttachWindow((HWND)wParam, ((CBT_CREATEWND *)lParam)->lpcs);
		break;
	case HCBT_DESTROYWND:
		break;
	}
	return CallNextHookEx(ghookCBT, nCode, wParam, lParam);
}
 
BOOL HOOKCBT::AttachWindow(HWND hWnd, CREATESTRUCT* pCreate)
{
	CHAR		szClassName[MAX_PATH]		= {0};
	HANDLE		hWndProc					= ::GetProp(hWnd, STR_SKINWND_PROP);

	if(NULL != hWndProc)
		return TRUE;

	GetClassName(hWnd, szClassName, sizeof(szClassName));
	if(stricmp("#32770", szClassName) == 0)
	{
		return HOOKCBT::AttachSkinWindow(hWnd, new CSkinDialog(hWnd));
	}

	return FALSE;
}
BOOL HOOKCBT::AttachSkinWindow(HWND hWnd, CSkinWnd* pSkinWnd)
{
	WNDPROC		wndPorc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)HOOKCBT::CallSkinWindowProc);

	::SetProp(hWnd, STR_SKINWND_PROP, wndPorc);
	::SetProp(hWnd, STR_SKINWND_PROP_PTR, (HANDLE)pSkinWnd);

	return TRUE;
}
LRESULT HOOKCBT::CallSkinWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	CSkinWnd*		pSkinWnd		= (CSkinWnd *)::GetProp(hWnd, STR_SKINWND_PROP_PTR);
	LRESULT			lRet			= 0;

	if(NULL != pSkinWnd && hWnd == pSkinWnd->m_hWnd)
	{
		if(FALSE != pSkinWnd->WindowProc(Msg, wParam, lParam, lRet))
			return lRet;
	}

	return CallWindowProc((WNDPROC)::GetProp(hWnd, STR_SKINWND_PROP), hWnd, Msg, wParam, lParam);
}