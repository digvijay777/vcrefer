#include "StdAfx.h"
#include "HOOKCBT.h"
#include "SkinDialog.h"

HHOOK				ghookCBT			= NULL;

//////////////////////////////////////////////////////////////////////////
// 初始化
BOOL SSkin::Install()
{
	if(NULL != ghookCBT)
		return FALSE;

	ghookCBT = SetWindowsHookEx(WH_CBT, SSkin::CBTProc, NULL, GetCurrentThreadId());
	return (NULL != ghookCBT);
}

BOOL SSkin::Uninstall()
{
	if(NULL == ghookCBT)
		return TRUE;

	if(FALSE != UnhookWindowsHookEx(ghookCBT))
		ghookCBT = NULL;

	return (NULL == ghookCBT);
}

//////////////////////////////////////////////////////////////////////////
// 创建钩子
LRESULT CALLBACK SSkin::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch(nCode)
	{
	case HCBT_CREATEWND:
		SSkin::AttachWindow((HWND)wParam, ((CBT_CREATEWND *)lParam)->lpcs);
		break;
	case HCBT_DESTROYWND:
		break;
	}
	return CallNextHookEx(ghookCBT, nCode, wParam, lParam);
}
 
BOOL SSkin::AttachWindow(HWND hWnd, CREATESTRUCT* pCreate)
{
	CHAR		szClassName[MAX_PATH]		= {0};
	HANDLE		hWndProc					= ::GetProp(hWnd, STR_SKINWND_PROP);

	if(NULL != hWndProc)
		return TRUE;

	GetClassName(hWnd, szClassName, sizeof(szClassName));
	if(stricmp("#32770", szClassName) == 0)
	{
		return SSkin::AttachSkinWindow(hWnd, new CSkinDialog(hWnd));
	}

	return FALSE;
}
BOOL SSkin::AttachSkinWindow(HWND hWnd, CSkinWnd* pSkinWnd)
{
	WNDPROC		wndPorc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SSkin::CallSkinWindowProc);

	::SetProp(hWnd, STR_SKINWND_PROP, wndPorc);
	::SetProp(hWnd, STR_SKINWND_PROP_PTR, (HANDLE)pSkinWnd);

	return TRUE;
}
LRESULT SSkin::CallSkinWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
