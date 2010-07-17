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
// ���öԻ���������ĸ߶�  wParam: DLGCAPTIONINFO		lParam: null
#define UM_GDIPDLG_SETCAPTION	UM_GDIPDIALOG+4
typedef struct tagDlgCaptionInfo
{
	RECT		rect;
}DLGCAPTIONINFO;


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
	inline BOOL Dlg_SetCaptionRect(HWND hWnd, int nLeft = -1, int nTop = -1, int nRight = -1, int nBottom = -1)
	{
		DLGCAPTIONINFO		info;

		info.rect.left = nLeft;
		info.rect.top = nTop;
		info.rect.right = nRight;
		info.rect.bottom = nBottom;
		return (BOOL)SendMessage(hWnd, UM_GDIPDLG_SETCAPTION, (WPARAM)&info, 0);
	}
}