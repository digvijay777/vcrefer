#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
// ��������
HWND CreateMyWindow(HWND hParent)
{
	extern HINSTANCE gInst;

	// EndDialog(NULL, 0);
	return CreateDialogA(gInst, MAKEINTRESOURCEA(IDD_DIALOG1), hParent, DialogProc);
}


