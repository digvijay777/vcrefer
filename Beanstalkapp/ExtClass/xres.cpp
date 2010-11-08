/*
 *	动态资源类操作
 *  
 */
#define _WIN32_WINNT		0x500

#include <afx.h>
#include <windows.h>
#include <string>
#include "xres.h"

//////////////////////////////////////////////////////////////////////////
// 对话框操作
/*
 *	独一无二的对话框
 */
// 独立对话框参数
typedef struct tagMsgboxParam{
	std::string		strTitle;
	std::string		strMsg;
	UINT			nType;
}MSGBOXPARAM;

// 独立对话框回调函数
INT_PTR CALLBACK AloneMsgbox_Proc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(WM_INITDIALOG == uMsg)
	{
		MSGBOXPARAM*		pParam		= (MSGBOXPARAM *)lParam;

		::SetWindowLong(hwndDlg, GWL_EXSTYLE
			, ::GetWindowLong(hwndDlg, GWL_EXSTYLE) | 0x80000);
		::SetLayeredWindowAttributes(hwndDlg, 0, 50, LWA_ALPHA);
		::SetWindowPos(hwndDlg, HWND_TOPMOST, 0, 0
			, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
		::PostMessage(hwndDlg, WM_USER+100, 0, lParam);
	}
	else if( (WM_USER + 100) == uMsg)
	{
		MSGBOXPARAM*		pParam		= (MSGBOXPARAM *)lParam;
		int				nRes		= 0;

		nRes = MessageBox(hwndDlg, pParam->strMsg.c_str()
			, pParam->strTitle.c_str(), pParam->nType);
		EndDialog(hwndDlg, nRes);
	}

	return 0;
}
// 独立对话框
int XRes::AloneMsgbox(HWND hWnd, LPCSTR lpText, LPCSTR lpTitle, UINT nType)
{
	static DLGTEMPLATE			dlgTmplate		= {0};
	MSGBOXPARAM					param;
	HINSTANCE					hInst;

	ASSERT(hWnd != NULL);

	dlgTmplate.cdit = 0;
	dlgTmplate.cx = GetSystemMetrics(SM_CXSCREEN);
	dlgTmplate.cy = GetSystemMetrics(SM_CYSCREEN);
	dlgTmplate.x = 0;
	dlgTmplate.y = 0;
	dlgTmplate.style = WS_POPUP | WS_VISIBLE;
	dlgTmplate.dwExtendedStyle = WS_EX_TOOLWINDOW;

	param.nType = nType | MB_TASKMODAL;
	param.strTitle = lpTitle;
	param.strMsg = lpText;
	
	hInst = (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE);

	return DialogBoxIndirectParamA(hInst, &dlgTmplate, NULL, AloneMsgbox_Proc, (LPARAM)&param);
}

/*
 *	延时功能的对话框
 *
 */
#define MB_TIMEOUT			32000
// 延时对话框回调函数
INT_PTR CALLBACK TimeoutMsgbox_Proc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(WM_INITDIALOG == uMsg)
	{
	}
	else if(WM_SYSCOMMAND == uMsg)
	{
		switch(wParam)
		{
		case SC_CLOSE:
			EndDialog(hwndDlg, IDCANCEL);
			break;
		}
	}

	return 0;
}
VOID CALLBACK MsgBoxCallback(LPHELPINFO lpHelpInfo)
{
	int	i = 0;

	Sleep(i);
}
// 延时对话框
int XRes::TimeoutMsgbox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpTitle, UINT nType, UINT nTimeout /* = INFINITE */)
{
/*
	DLGTEMPLATE*		pDlgTmplate			= NULL;
	HINSTANCE			hInst				= (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE);
	MSGBOXPARAM			param;

	//return DialogBoxIndirectParamA(hInst, pDlgTmplate, hWnd, DelayMsgbox_Proc, (LPARAM)&param);
	MSGBOXPARAMSA		msgbox				= {0};

	msgbox.cbSize = sizeof(msgbox);
	msgbox.dwStyle = nType;
	msgbox.hwndOwner = hWnd;
	msgbox.hInstance = (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE);
	msgbox.lpszText = lpText;
	msgbox.lpszCaption = lpTitle;
	msgbox.lpfnMsgBoxCallback = MsgBoxCallback;

	return MessageBoxIndirect(&msgbox);
*/
/*
	typedef int (WINAPI *fnMessageBoxTimeoutA)(HWND, LPCSTR, LPCSTR, UINT, WORD, DWORD);

	HINSTANCE		hUser32		= NULL;
	int				nRet		= 0;
	BOOL			bSuccess	= FALSE;

	hUser32 = LoadLibrary("user32.dll");
	if(NULL != hUser32)
	{
		fnMessageBoxTimeoutA		MessageBoxTimeoutA		= NULL;

		MessageBoxTimeoutA = (fnMessageBoxTimeoutA)GetProcAddress(hUser32, "MessageBoxTimeoutA");
		if(NULL != MessageBoxTimeoutA)
		{
			bSuccess = TRUE;
			nRet = MessageBoxTimeoutA(hWnd, lpText, lpTitle, nType, 0, nTimeout);
		}
		FreeLibrary(hUser32);
	}
	if(FALSE != bSuccess)
		return nRet;
	// 调用失败
	
	return MessageBox(hWnd, lpText, lpTitle, nType);*/

	DLGTEMPLATE*		pDlg				= NULL;
	DLGITEMTEMPLATE*	pIconItem			= NULL;
	DLGITEMTEMPLATE*	pTextItem			= NULL;
	DLGITEMTEMPLATE*	pBtnItem[4]			= {0};
	HGLOBAL				hGlobal				= NULL;
	DLGTEMPLATE*		pDlgTemplate		= NULL;
	BYTE*				pPos				= NULL;
	int					nRet				= MB_TIMEOUT;

	// 分配空间
	hGlobal = GlobalAlloc(GPTR, 1024);
	if(NULL == hGlobal)
		return -1;
	pDlgTemplate = (DLGTEMPLATE *)GlobalLock(hGlobal);
	if(NULL == pDlgTemplate)
	{
		GlobalFree(hGlobal);
		return -1;
	}
	// 构建窗体
	pPos = (BYTE *)pDlgTemplate;
	pDlg = (DLGTEMPLATE *)pPos;
	wcscpy((WCHAR*)(pPos + sizeof(DLGTEMPLATE) + sizeof(WCHAR) + sizeof(WCHAR)), lpTitle);
	pDlg->cx = 100;
	pDlg->cy = 100;
	pDlg->style = DS_MODALFRAME | DS_3DLOOK | WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION;

	// 图标
	pPos += (sizeof(DLGTEMPLATE) + sizeof(WCHAR) + sizeof(WCHAR) + (wcslen(lpTitle) + 1));
	if(0 != ((MB_ICONERROR | MB_ICONQUESTION | MB_ICONWARNING | MB_ICONINFORMATION) & nType))
	{
		pIconItem = (DLGITEMTEMPLATE *)pPos;
		pIconItem->cx = 32;
		pIconItem->cy = 32;
		pIconItem-
	}
	// 弹出对话框
	HINSTANCE			hInst		= (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE);

	nRet = DialogBoxIndirect(hInst, pDlg, hWnd, TimeoutMsgbox_Proc);
	// 清理
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return nRet;
}

