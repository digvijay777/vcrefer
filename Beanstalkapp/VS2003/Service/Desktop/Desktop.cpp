// Desktop.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Desktop.h"
#define MAX_LOADSTRING 100

// 此代码模块中包含的函数的前向声明:
HDESK		hDesktopOrig		= NULL;
HDESK		hDesktop			= NULL;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	::MessageBox(NULL, "This is on the desktop", "Hey", MB_OK);
	return 0;
}

DWORD WINAPI AlternateDesktopThreadProc(LPVOID lpParameter)
{
	::SetThreadDesktop(hDesktop);
// 	STARTUPINFO			si		= {0};
// 	PROCESS_INFORMATION	pi		= {0};
// 
// 	si.cb = sizeof(STARTUPINFO);
// 	si.lpDesktop = "MyDesktop";
// 
// 	if(FALSE != CreateProcess(NULL, "C:\\windows\\explorer.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
// 	{
// 		WaitForSingleObject(pi.hProcess, INFINITE);
// 		CloseHandle(pi.hProcess);
// 		CloseHandle(pi.hThread);
// 	}
	::MessageBox(NULL, "This is on an  alternate desktop", "Hey", MB_OK);
	::MessageBox(NULL, "To switch desktop", "Click OK", MB_OK);
	::SwitchDesktop(hDesktopOrig);
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	hDesktopOrig = ::GetThreadDesktop(::GetCurrentThreadId());
	
	HANDLE		hThread = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	hDesktop = ::CreateDesktop("MyDesktop", NULL, NULL, 0, DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP, NULL);

	STARTUPINFO		si		= {0};

	GetStartupInfo(&si);
	HANDLE		hAltThread = ::CreateThread(NULL, 0, AlternateDesktopThreadProc, NULL, 0, NULL);
// 	WaitForSingleObject(hAltThread, INFINITE);
	::MessageBox(NULL, "To switch desktops", "Click OK", MB_OK);
	::SwitchDesktop(hDesktop);
	
	::MessageBox(NULL, "To shutdown this app", "Click OK", MB_OK);
	::CloseHandle(hThread);
	::CloseHandle(hAltThread);
	::CloseDesktop(hDesktop);
	return 0;
}
