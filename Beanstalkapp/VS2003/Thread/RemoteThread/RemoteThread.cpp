// RemoteThread.cpp : ���� DLL Ӧ�ó������ڵ㡣
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

// ���ǵ���������һ��ʾ��
REMOTETHREAD_API int nRemoteThread=0;

// ���ǵ���������һ��ʾ����
REMOTETHREAD_API int fnRemoteThread(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� RemoteThread.h
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
		sprintf(szMsg, "��������ʧ��:%d", GetLastError());
		::MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	}
	return 0;
}