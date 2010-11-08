#pragma once
#include "SkinWnd.h"

extern HHOOK		ghookCBT;


namespace HOOKCBT
{
	LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

	BOOL	AttachWindow(HWND hWnd, CREATESTRUCT* pCreate);
	BOOL	DetachWindow();

	BOOL	AttachSkinWindow(HWND hWnd, CSkinWnd* pSkinWnd);
	LRESULT CallSkinWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
}