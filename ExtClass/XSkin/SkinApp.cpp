#include "StdAfx.h"
#include "SkinApp.h"
#include "SkinDialog.h"

CSkinApp* CSkinApp::s_pSkinApp		= NULL;

CSkinApp::CSkinApp(void)
{
	ASSERT(NULL == s_pSkinApp);
	s_pSkinApp = this;
	m_hhook = NULL;
}

CSkinApp::~CSkinApp(void)
{
	SKINWNDDATAPTR		wnd;

	for(wnd = m_mapWnds.begin()
		; m_mapWnds.end() != wnd
		; wnd++)
	{
		delete wnd->second.pSkinWnd;
	}

	m_mapWnds.clear();
}

BOOL CSkinApp::Install()
{
	if(NULL != m_hhook)
		return FALSE;

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &gdiplusStartupInput, NULL);

	m_hhook = SetWindowsHookEx(WH_CBT, CSkinApp::CBTProc, NULL, GetCurrentThreadId());
	return (NULL != m_hhook);
}

BOOL CSkinApp::Uninstall()
{
	if(NULL == m_hhook)
		return TRUE;

	if(FALSE != UnhookWindowsHookEx(m_hhook))
	{
		m_hhook = NULL;
		GdiplusShutdown(m_pGdiToken);
	}

	return (NULL == m_hhook);
}

LRESULT CALLBACK CSkinApp::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT(NULL != s_pSkinApp);

	if(NULL == s_pSkinApp)
		return 0;

	switch(nCode)
	{
	case HCBT_CREATEWND:
		s_pSkinApp->AttachWindow((HWND)wParam, ((CBT_CREATEWND *)lParam)->lpcs);
		break;
	case HCBT_DESTROYWND:
		break;
	}

	return ::CallNextHookEx(s_pSkinApp->m_hhook, nCode, wParam, lParam);
}

LRESULT CALLBACK CSkinApp::CallSkinWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(NULL == s_pSkinApp)
	{
		ASSERT(NULL != s_pSkinApp);
		return 0;
	}

	CSkinWnd*		pSkinWnd		= s_pSkinApp->GetSkinWnd(hWnd);
	LRESULT			lRet			= 0;

	if(NULL != pSkinWnd && hWnd == pSkinWnd->m_hWnd)
	{
		if(FALSE != pSkinWnd->WindowProc(Msg, wParam, lParam, lRet))
			return lRet;
	}

	return CallWindowProc(s_pSkinApp->GetOldWndProc(hWnd), hWnd, Msg, wParam, lParam);
}

BOOL CSkinApp::AttachSkinWindow(HWND hWnd, CSkinWnd* pSkinWnd)
{
	SKINWNDDATA		data;
	SKINWNDDATAPTR	item;

	item = m_mapWnds.find(hWnd);
	if(m_mapWnds.end() != item)
	{
		ASSERT(m_mapWnds.end() == item);
		return FALSE;
	}

	data.pSkinWnd = pSkinWnd;
	data.wndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)CallSkinWindowProc);
	
	m_mapWnds[hWnd] = data;

	return TRUE;
}
CSkinWnd* CSkinApp::GetSkinWnd(HWND hWnd)
{
	SKINWNDDATAPTR			item;
	
	item = m_mapWnds.find(hWnd);
	if(m_mapWnds.end() == item)
		return NULL;

	return item->second.pSkinWnd;
}
WNDPROC CSkinApp::GetOldWndProc(HWND hWnd)
{
	SKINWNDDATAPTR			item;

	item = m_mapWnds.find(hWnd);
	if(m_mapWnds.end() == item)
		return NULL;

	return item->second.wndProc;
}

BOOL CSkinApp::AttachWindow(HWND hWnd, CREATESTRUCT* pCreate)
{
	CHAR		szClassName[MAX_PATH]		= {0};
	HANDLE		hWndProc					= GetOldWndProc(hWnd);

	if(NULL != hWndProc)
		return TRUE;

	GetClassName(hWnd, szClassName, sizeof(szClassName));
	if(stricmp("#32770", szClassName) == 0)
	{
		return AttachSkinWindow(hWnd, new CSkinDialog(hWnd));
	}
// 	if(stricmp("system32menu))
// 	else
// 	{
// 		return AttachSkinWindow(hWnd, new CSkinWnd(hWnd));
// 	}
	

	return FALSE;
}

BOOL CSkinApp::DetachWindow()
{
	return FALSE;
}
