/*
 *	GDIPWnd APP
 */
#include "stdafx.h"
#include "GdipApp.h"
#include "GdipDialog.h"

CGdipApp theGdipApp;

CGdipApp::CGdipApp()
{
	m_pGdiToken = NULL;
	m_hhook = NULL;
}

CGdipApp::~CGdipApp()
{

}

// 初始化库
BOOL CGdipApp::InitGdipWndApp()
{
	// 初始化GDI+
	if(NULL != m_hhook)
	{
		SetLastError(ERROR_FILE_EXISTS);
		return FALSE;
	}

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &gdiplusStartupInput, NULL);

	// 设置窗体钩子
	m_hhook = SetWindowsHookEx(WH_CBT, CGdipApp::CBTProc, NULL, GetCurrentThreadId());

	return (NULL != m_hhook);
}

// Attach Window
BOOL CGdipApp::AttachWindow(HWND hWnd, CREATESTRUCT* pCreate)
{
	CHAR		szClassName[MAX_PATH]		= {0};

	GetClassName(hWnd, szClassName, sizeof(szClassName));

	if(stricmp("#32770", szClassName) == 0)
	{
		// 钩住对话框
		return AttachGdipWnd(hWnd, new CGdipDialog(hWnd));
	}

	return FALSE;
}

BOOL CGdipApp::DetachWindow(HWND hWnd)
{
	CGdipWnd*	pWnd		= (CGdipWnd *)GetProp(hWnd, GDIPAPP_WNDPTR);

	if(NULL != pWnd)
		delete pWnd;

	return TRUE;
}

// 设置窗口过程
BOOL CGdipApp::AttachGdipWnd(HWND hWnd, CGdipWnd* pWnd)
{
	WNDPROC		wndOld		= (WNDPROC)(__int64)GetWindowLong(hWnd, GWL_WNDPROC);
	WNDPROC		wndGdip		= CallWindowProc;

	if(wndOld == wndGdip)
		return TRUE;

	wndOld = (WNDPROC)(__int64)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)(__int64)wndGdip);
	if(NULL == wndOld)
		return FALSE;

	SetProp(hWnd, GDIPAPP_WNDPROC, (HANDLE)wndOld);
	SetProp(hWnd, GDIPAPP_WNDPTR, (HANDLE)pWnd); 

	return TRUE;
}

// CBTProc
LRESULT CGdipApp::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch(nCode)
	{
	case HCBT_CREATEWND:
		theGdipApp.AttachWindow((HWND)wParam, ((CBT_CREATEWND *)lParam)->lpcs);
		break;
	case HCBT_DESTROYWND:
		theGdipApp.DetachWindow((HWND)wParam);
		break;
	}

	return ::CallNextHookEx(theGdipApp.m_hhook, nCode, wParam, lParam);
}

LRESULT CGdipApp::CallWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC		wndProc		= (WNDPROC)GetProp(hWnd, GDIPAPP_WNDPROC);
	CGdipWnd*	pWnd		= (CGdipWnd *)GetProp(hWnd, GDIPAPP_WNDPTR);
	LRESULT		lResult		= 0;

	if(NULL == wndProc)
		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
	if(NULL == pWnd)
		return ::CallWindowProc(wndProc, hWnd, Msg, wParam, lParam);

	if(hWnd != pWnd->m_hWnd)
	{
		ATLASSERT(hWnd != pWnd->m_hWnd);
		return ::CallWindowProc(wndProc, hWnd, Msg, wParam, lParam);
	}

	// 调用自定义过程
	if(FALSE != pWnd->WindowProc(Msg, wParam, lParam, &lResult))
		return lResult;

	return ::CallWindowProc(wndProc, hWnd, Msg, wParam, lParam);
}