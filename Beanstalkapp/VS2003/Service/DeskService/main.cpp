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
 *	��������
 *
 *  ˵��:��������ʱ�����Զ��˳�
 */
void ServerMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	// ��ʼ����һ����Ϣ
	g_dwMainThreadID = GetCurrentThreadId();
	PostThreadMessage(g_dwMainThreadID, UM_HOOK, 0, 0);

	// ��Ϣѭ��
	MSG		msg;
	while(true)
	{
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			switch(msg.message)
			{
			case UM_HOOK:			// ��װ����
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
			case UM_UNHOOK:			// ���ع���
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
			case UM_REHOOK:			// ���¹ҽӹ���
				{
					PostThreadMessage(g_dwMainThreadID, UM_UNHOOK, 0, 0);
					PostThreadMessage(g_dwMainThreadID, UM_HOOK, 0, 0);
				}
				break;
			}
			// ��ʱ��

			// �����н���
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
 *	���̹��ӻص�����
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
 *	��������
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

	// ����EXPLORER����
	ExecuteConfig(" /explorer", hDesk);

	return hDesk;
}
/*
 *	�õ��û�Token
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
 *	ִ�������ļ�
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
		// ��������
		CreateEnvironmentBlock(&lpEnvironment, hToken, TRUE);
		// ��������
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