#pragma once
#include <comdef.h>

// 初始化GDIPWND库
BOOL		InitGdipWndApp();

//////////////////////////////////////////////////////////////////////////
// 各类消息定义

// GDIPBASE 消息从WM_USER+1024*3开如
#define UM_GDIPBASE				(WM_USER + 1024 * 4)

//////////////////////////////////////////////////////////////////////////
// 窗口消息
#define UM_GDIPWND				UM_GDIPWND+1

//////////////////////////////////////////////////////////////////////////
// 对话框消息
#define UM_GDIPDIALOG			UM_GDIPBASE+51

// 设置对话框背景 
// wParam:图片路径		lParam:为空
#define UM_GDIPDLG_SETBK		UM_GDIPDIALOG+1
// wParam:资源ID			lParam:为资类型
#define UM_GDIPDLG_SETBK2		UM_GDIPDIALOG+2
// wParam: ARGB
#define UM_GDIPDLG_SETBKCOLOR	UM_GDIPDIALOG+3




//////////////////////////////////////////////////////////////////////////

namespace GdipWnd
{
	// 设置背景
	inline BOOL Dlg_SetBk(HWND hWnd, UINT nID, LPCTSTR lpType)
	{
		return (BOOL)SendMessage(hWnd, UM_GDIPDLG_SETBK2, (WPARAM)MAKEINTRESOURCE(nID), (LPARAM)lpType);
	}
	inline BOOL Dlg_SetBk(HWND hWnd, LPCTSTR lpPath)
	{
		return (BOOL)SendMessage(hWnd, UM_GDIPDLG_SETBK, (WPARAM)lpPath, NULL);
	}
	inline BOOL Dlg_SetBkColor(HWND hWnd, DWORD dwColor)
	{
		return (BOOL)SendMessage(hWnd, UM_GDIPDLG_SETBKCOLOR, (WPARAM)dwColor, 0);
	}
}