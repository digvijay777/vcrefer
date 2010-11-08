#include "stdafx.h"
#include "control.h"
#include <Tlhelp32.h>
#include <shlwapi.h>
#include <Userenv.h>
// #include <atlstr.h>
// #include <afx.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Userenv.lib")

HHOOK		g_hook				= NULL;
DWORD		g_dwMainThreadID	= 0;
HDESK		g_hDownDesk			= NULL;
HDESK		g_hLeftDesk			= NULL;
HDESK		g_hRightDesk		= NULL;
HDESK		g_hUpDesk			= NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam);
HDESK CreateDesktopWithExplorer(LPCTSTR lpDesk);
HANDLE	GetUserToken();
BOOL ExecuteConfig(LPCSTR  lpParam, HDESK hDesk);
/*
 *	服务主体
 *
 *  说明:函数结束时服务将自动退出
 */
void ServerMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	// 开始寄送一个消息
	g_dwMainThreadID = GetCurrentThreadId();
	PostThreadMessage(g_dwMainThreadID, UM_HOOK, 0, 0);

	// 消息循环
	MSG		msg;
	while(true)
	{
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			switch(msg.message)
			{
			case UM_HOOK:			// 安装钩子
				{
					if(NULL == g_hook)
					{
						do 
						{
							g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
							Sleep(1);
						} while (NULL == g_hook);
					}
				}
				break;
			case UM_UNHOOK:			// 御载钩子
				{
					if(NULL != g_hook)
					{
						while(!UnhookWindowsHookEx(g_hook))
						{
							Sleep(1);
						}
						g_hook = NULL;
					}
				}
				break;
			case UM_REHOOK:			// 重新挂接钩子
				{
					PostThreadMessage(g_dwMainThreadID, UM_UNHOOK, 0, 0);
					PostThreadMessage(g_dwMainThreadID, UM_HOOK, 0, 0);
				}
				break;
			}
			// 定时器

			// 不能有界面
// 			TranslateMessage(&msg);
// 			DispatchMessage(&msg);
		}
		else
		{
			Sleep(1);
		}
	}
}

/*
 *	键盘钩子回调函数
 */
LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if(HC_ACTION == code)
	{
		if(WM_KEYDOWN == wParam || WM_SYSKEYDOWN == wParam)
		{
			PKBDLLHOOKSTRUCT	pKeyHook	= (PKBDLLHOOKSTRUCT)lParam;
			
			
			int		vkLWin	= GetAsyncKeyState(VK_LWIN);
			int		vkRWin	= GetAsyncKeyState(VK_RWIN);

			if(	(vkLWin >> 7 || vkRWin >> 7) )
			{
				switch(pKeyHook->vkCode)
				{
				case VK_UP:
					{
						g_hUpDesk = CreateDesktopWithExplorer("Default_2");
						if(SetThreadDesktop(g_hUpDesk))
						{
							SwitchDesktop(g_hUpDesk);
							PostThreadMessage(g_dwMainThreadID, UM_REHOOK, 0, 0);
						}
					}
					return 1;
				case VK_DOWN:
					{
						g_hDownDesk = CreateDesktopWithExplorer("Default");
						if(SetThreadDesktop(g_hDownDesk))
						{
							SwitchDesktop(g_hDownDesk);
							PostThreadMessage(g_dwMainThreadID, UM_REHOOK, 0, 0);
						}
					}
					return 1;
				case VK_LEFT:
					{
						g_hLeftDesk = CreateDesktopWithExplorer("Default_1");
						if(SetThreadDesktop(g_hLeftDesk))
						{
							SwitchDesktop(g_hLeftDesk);
							PostThreadMessage(g_dwMainThreadID, UM_REHOOK, 0, 0);
						}
					}
					return 1;
				case VK_RIGHT:
					{
						g_hRightDesk = CreateDesktopWithExplorer("Default_3");
						if(SetThreadDesktop(g_hRightDesk))
						{
							SwitchDesktop(g_hRightDesk);
							PostThreadMessage(g_dwMainThreadID, UM_REHOOK, 0, 0);
						}
					}
					return 1;
				}
			}
		}
	}

	return ::CallNextHookEx(g_hook, code, wParam, lParam);
}

/*
 *	创建桌面
 */
HDESK CreateDesktopWithExplorer(LPCTSTR lpDesk)
{
	HDESK		hDesk	= NULL;

	hDesk = OpenDesktop(lpDesk, 0, FALSE, MAXIMUM_ALLOWED);

	if(NULL != hDesk)
	{
		ExecuteConfig(" /explorer", hDesk);
		return hDesk;
	}

	hDesk = CreateDesktop(lpDesk, NULL, NULL, 0, MAXIMUM_ALLOWED, NULL);
	if(NULL == hDesk)
		return NULL;

	// 创建EXPLORER进程
	ExecuteConfig(" /explorer", hDesk);

	return hDesk;
}
/*
 *	得到用户Token
 */
HANDLE GetUserToken()
{
	HANDLE					hProcessSnap		= NULL;     
	PROCESSENTRY32			pe32				= {0};
	HANDLE					hToken				= NULL;
	HANDLE					hRetToken			= NULL;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,   0);
	if(INVALID_HANDLE_VALUE == hProcessSnap)     
		return NULL;     

	pe32.dwSize = sizeof(PROCESSENTRY32);     
	if(Process32First(hProcessSnap, &pe32))     
	{   
		do     
		{   
			if(!stricmp(pe32.szExeFile, "explorer.exe"))   
			{   
				HANDLE   hProcess		= OpenProcess(PROCESS_QUERY_INFORMATION,   
														FALSE,pe32.th32ProcessID);   
				OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);   
				break;   
			}   
		}     
		while(Process32Next(hProcessSnap, &pe32));     
	}     


	CloseHandle(hProcessSnap);
	DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &hRetToken);
	CloseHandle(hToken);
	return hRetToken;
}

/*
 *	执行配置文件
 */
BOOL ExecuteConfig(LPCSTR lpParam, HDESK hDesk)
{
	HANDLE		hToken				= NULL;
	BOOL		bRet				= FALSE;
	CHAR		szDesk[MAX_PATH]	= {0};

	if(!GetUserObjectInformation(hDesk, UOI_NAME, szDesk, MAX_PATH, NULL))
		return FALSE;

	hToken = GetUserToken();
	if(NULL != hToken && ImpersonateLoggedOnUser(hToken))
	{
		STARTUPINFO				si						= {0};
		PROCESS_INFORMATION		pi						= {0};
		CHAR					szExplorer[1024]		= {0};
		LPVOID					lpEnvironment			= NULL;

		si.cb = sizeof(STARTUPINFO);
		si.lpDesktop = szDesk;
		GetModuleFileName(NULL, szExplorer, MAX_PATH);
		// 环境变量
		CreateEnvironmentBlock(&lpEnvironment, hToken, TRUE);
		// 创建进程
		if(CreateProcessAsUser(hToken, szExplorer, (LPSTR)lpParam, NULL, NULL, FALSE, 
			0, lpEnvironment, NULL, &si, &pi))
		{
			bRet = TRUE;
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		DestroyEnvironmentBlock(lpEnvironment);
		RevertToSelf();
		CloseHandle(hToken);
	}

	return bRet;
}