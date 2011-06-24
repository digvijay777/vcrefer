// #include <Windows.h>
#include "stdafx.h"
#include <stdio.h>
//#include "../../../common/DebugLog.h"

#include <Wtsapi32.h>

#pragma comment(lib, "Wtsapi32.lib")

SERVICE_STATUS				g_sServiceStatus			= {0};
SERVICE_STATUS_HANDLE		g_hStatus					= NULL;
TCHAR						g_szServiceName[]			= "MyFirstServiceTest";
HWND						g_hMsgWnd					= NULL;
HINSTANCE					g_hInst						= NULL;

void	WINAPI MyServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID	WINAPI ControlHandler(	DWORD fdwControl);
int		InitService();
LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
VOID	CreateSampleService();
void	ServiceTestCreateProcess();
void	ServiceTestMsgBox();

//CDebugLog	g_debuglog("C:\\debug.log");
//////////////////////////////////////////////////////////////////////////
// main函数
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPTSTR lpCmdLine, 
				   int nShowCmd )
{
  	g_hInst = hInstance;
//	g_debuglog.Write("Enter WinMain.");
	if(_tcsstr(lpCmdLine, "/create") > 0)
	{
		CreateSampleService();
		return 0;
	}

	SERVICE_TABLE_ENTRY		ServiceTable[2]		= {0};
	ServiceTable[0].lpServiceName = g_szServiceName;
	ServiceTable[0].lpServiceProc = MyServiceMain;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	// 启动服务的控制分派线程
	if(!StartServiceCtrlDispatcher(ServiceTable))
	{
//		g_debuglog.Write("StartServiceCtrlDispatcher filead.");
	}
//	g_debuglog.Write("End WinMain");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 服务入口
void WINAPI MyServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
//	__asm int 3

//	g_debuglog.Write("Enter MyServiceMain");
	int			nErr			= 0;
	
	g_sServiceStatus.dwServiceType = SERVICE_WIN32;
	g_sServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_sServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP
		| SERVICE_ACCEPT_SHUTDOWN;
	g_sServiceStatus.dwWin32ExitCode = 0;
	g_sServiceStatus.dwServiceSpecificExitCode = 0;
	g_sServiceStatus.dwCheckPoint = 0;
	g_sServiceStatus.dwWaitHint = 0;

	g_hStatus = RegisterServiceCtrlHandler(g_szServiceName, 
		ControlHandler);
	if(g_hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		TRACE("[MY_SERVICE] RegisterServiceCtrlHandler \
			failed %d\n", GetLastError()); 
//		g_debuglog.Write("RegisterServiceCtrlHandler failed");
		return;
	}

	// 初始化服务
	nErr = InitService();
	if(nErr)
	{
		g_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_sServiceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(g_hStatus, &g_sServiceStatus);
//		g_debuglog.Write("InitService failed.");
		return;
	}

	// 运行服务
	g_sServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_hStatus, &g_sServiceStatus);
	MSG		msg;
	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// 退出服务
	g_sServiceStatus.dwWin32ExitCode = 0;
	g_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_hStatus, &g_sServiceStatus);
}

//////////////////////////////////////////////////////////////////////////
// 初始化服务
int InitService2()
{
	WNDCLASS	wndClass	= {0};
	
	wndClass.hInstance = g_hInst;
	wndClass.lpfnWndProc = ServerWndProc;
	wndClass.lpszClassName = g_szServiceName;
	
	int n = RegisterClass(&wndClass);
	g_hMsgWnd = CreateWindow(g_szServiceName, "", WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, NULL, g_hInst, NULL);
	if(NULL == g_hMsgWnd)
		return 0;
	return 0;
}

int InitService()
{
// 	_asm int 3
	// RtlOpenCurrentUser
	// ServiceTestCreateProcess();
	ServiceTestMsgBox();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// 服务控制
VOID	WINAPI ControlHandler(	DWORD fdwControl)
{
	switch(fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		PostMessage(g_hMsgWnd, WM_CLOSE, 0, 0);
		break;
	default:
		SetServiceStatus(g_hStatus, &g_sServiceStatus);
		break;
	}
//	g_debuglog.Write("ControlHandler");
}

//////////////////////////////////////////////////////////////////////////
// 窗口过程
LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
// 	if(hwnd != g_hMsgWnd)
// 		return 0;
	switch(uMsg)
	{
	case WM_CLOSE:
		//DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 创建服务
VOID CreateSampleService() 
{ 
	TCHAR lpszBinaryPathName[MAX_PATH] = {0};
	SC_HANDLE schService;

	GetModuleFileName(NULL, lpszBinaryPathName, sizeof(lpszBinaryPathName));

	SC_HANDLE schSCManager = OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	schService = CreateService( 
		schSCManager,              // SCManager database 
		g_szServiceName,              // name of service 
		"我的第一个服务",           // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
		SERVICE_DEMAND_START,      // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		lpszBinaryPathName,        // service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
}



//////////////////////////////////////////////////////////////////////////
// 测试启动进程
void ServiceTestCreateProcess()
{
	WTS_SESSION_INFO*	pwtsSessionInfo;
	DWORD				dwCount				= 0;
	HANDLE				hToken				= NULL;
	HANDLE				hDumToken			= NULL;

	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, 
		&pwtsSessionInfo, &dwCount);

	for(DWORD i = 0; i < dwCount; i++)
	{
		if(WTSActive == pwtsSessionInfo[i].State)
		{
			WTSQueryUserToken(pwtsSessionInfo[i].SessionId, &hToken);
			break;
		}
	}

	if(NULL == hToken)
		return;

// 	if(FALSE == DuplicateToken(hToken, SecurityAnonymous, &hDumToken))
// 		return;


	STARTUPINFO				si;
	PROCESS_INFORMATION		pi;

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));

	si.cb = sizeof(si);
	if(TRUE == CreateProcessAsUser(hToken, NULL, "c:\\windows\\notepad.exe", 
		NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	CloseHandle(hToken);
// 	CloseHandle(hDumToken);
}

// 弹出对话框
void ServiceTestMsgBox()
{
	WTS_SESSION_INFO*	pwtsSessionInfo;
	DWORD				dwCount				= 0;
	HANDLE				hToken				= NULL;
	HANDLE				hDumToken			= NULL;
	HWINSTA				hWinSta				= NULL;
	CString				str;
	CString				strTemp;


	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, 
		&pwtsSessionInfo, &dwCount);

	for(DWORD i = 0; i < dwCount; i++)
	{
		strTemp.Format("[%d]:%s\n", i, pwtsSessionInfo[i].pWinStationName);
		str += strTemp;
		if(WTSActive == pwtsSessionInfo[i].State && NULL == hToken)
		{
			WTSQueryUserToken(pwtsSessionInfo[i].SessionId, &hToken);
// 			hWinSta = OpenWindowStation("winsta0"/*pwtsSessionInfo[i].pWinStationName*/, FALSE, WINSTA_ACCESSCLIPBOARD   |
// 				WINSTA_ACCESSGLOBALATOMS |
// 				WINSTA_CREATEDESKTOP     |
// 				WINSTA_ENUMDESKTOPS      |
// 				WINSTA_ENUMERATE         |
// 				WINSTA_EXITWINDOWS       |
// 				WINSTA_READATTRIBUTES    |
// 				WINSTA_READSCREEN        |
// 				WINSTA_WRITEATTRIBUTES);
// 			strTemp.Format("[HWINSTA]: %d[%d]\n", hWinSta, GetLastError());
// 			str += strTemp;
			//break;
		}
	}

	if(NULL == hToken)
		return;

	HDESK			hDeskTop		= NULL;

//	ImpersonateLoggedOnUser(hToken);

/*
	if(NULL != hWinSta)
	{
		HDESK		hDesk		= NULL;

		if(FALSE == SetProcessWindowStation(hWinSta))
		{
			strTemp.Format("[SetProcessWindowStation] [%d]\n", GetLastError());
			str += strTemp;
		}
// 		hDesk = OpenDesktop("default", 0, FALSE,  DESKTOP_CREATEMENU |
// 			DESKTOP_CREATEWINDOW |
// 			DESKTOP_ENUMERATE    |
// 			DESKTOP_HOOKCONTROL  |
// 			DESKTOP_JOURNALPLAYBACK |
// 			DESKTOP_JOURNALRECORD |
// 			DESKTOP_READOBJECTS |
// 			DESKTOP_SWITCHDESKTOP |
// 			DESKTOP_WRITEOBJECTS);
// 		strTemp.Format("[HDESK]: %d[%d]\n", hDesk, GetLastError());
// 		str += strTemp;
// 		SetThreadDesktop(hDesk);
	}*/



//	ImpersonateLoggedOnUser(hToken);

	
	// 以用户身份启动进程成功代码
	STARTUPINFO					si		= {0};
	PROCESS_INFORMATION			pi		= {0};

	if(FALSE == CreateProcessAsUser(hToken, "notepad.exe", NULL, NULL
		, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		strTemp.Format("[CreateProcessAsUser]: [%d]\n", GetLastError());
		str += strTemp;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	
/*
	//查找窗体
		HWND					hWnd		= FindWindowEx(NULL, NULL, "_skynet_mon_mgr_", "_skynet_mon_mgr_");
		COPYDATASTRUCT			cpData		= {0};
	
		strTemp.Format("[FindWindowEx]: %d[%d]\n", hWnd, GetLastError());
		str += strTemp;
		cpData.dwData = 0;
		cpData.cbData = str.GetLength()+1;
		cpData.lpData = str.GetBuffer();
		SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cpData);*/


	MessageBox(NULL, str.GetBuffer(), "Service", MB_OK|MB_ICONERROR);

	CloseHandle(hToken);
	RevertToSelf();
}