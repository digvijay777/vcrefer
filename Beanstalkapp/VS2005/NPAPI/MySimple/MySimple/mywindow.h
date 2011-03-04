#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
// ´´½¨´°Ìå
HWND CreateMyWindow(HWND hParent)
{
	extern HINSTANCE gInst;

	return CreateDialogA(gInst, MAKEINTRESOURCEA(IDD_DIALOG1), hParent, DialogProc);
}


