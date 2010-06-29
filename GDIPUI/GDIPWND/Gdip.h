#pragma once
#include <comdef.h>

// ��ʼ��GDIPWND��
BOOL		InitGdipWndApp();

//////////////////////////////////////////////////////////////////////////
// ������Ϣ����

// GDIPBASE ��Ϣ��WM_USER+1024*3����
#define UM_GDIPBASE				(WM_USER + 1024 * 4)

//////////////////////////////////////////////////////////////////////////
// ������Ϣ
#define UM_GDIPWND				UM_GDIPWND+1

//////////////////////////////////////////////////////////////////////////
// �Ի�����Ϣ
#define UM_GDIPDIALOG			UM_GDIPBASE+51

// ���öԻ��򱳾� 
// wParam:ͼƬ·��		lParam:Ϊ��
#define UM_GDIPDLG_SETBK		UM_GDIPDIALOG+1
// wParam:��ԴID			lParam:Ϊ������
#define UM_GDIPDLG_SETBK2		UM_GDIPDIALOG+2
// wParam: ARGB
#define UM_GDIPDLG_SETBKCOLOR	UM_GDIPDIALOG+3




//////////////////////////////////////////////////////////////////////////

namespace GdipWnd
{
	// ���ñ���
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