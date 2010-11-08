// RemoteThread.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "RemoteThread.h"
#include "stdio.h"
#include "windows.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// 这是导出变量的一个示例
REMOTETHREAD_API int nRemoteThread=0;

// 这是导出函数的一个示例。
REMOTETHREAD_API int fnRemoteThread(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 RemoteThread.h
CRemoteThread::CRemoteThread()
{ 
	return; 
}

REMOTETHREAD_API DWORD WINAPI ThreadRemoteFn()
{
	STARTUPINFO			si		= {0};
	PROCESS_INFORMATION	pi		= {0};
	BOOL				bRes	= FALSE;
	
    si.cb = sizeof(STARTUPINFO);
	bRes = CreateProcess("C:\\Windows\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(TRUE == bRes)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		CHAR		szMsg[1024]		= {0};
		sprintf(szMsg, "启动进程失败:%d", GetLastError());
		::MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	}
	return 0;
}