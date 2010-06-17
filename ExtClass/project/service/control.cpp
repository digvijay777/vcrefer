#include "stdafx.h"
#include "control.h"

static SERVICE_STATUS				g_sServiceStatus			= {0};
static SERVICE_STATUS_HANDLE		g_hStatus					= NULL;

/*
 *	函数主体
 */
int main(int argc, char* argv[])
{
	if(argc > 1 && (argv[1][0] != '/' || argv[1][0] != '-'))
	{
		return ERROR_INVALID_PARAMETER;
	}

	PARSE_PARAMETER("create", ServiceCreate);
	PARSE_PARAMETER("start", ServiceStart);

	// Start service's thread.
	SERVICE_TABLE_ENTRY		ServiceTable[2]		= {0};

	ServiceTable[0].lpServiceName = str_ServiceName;
	ServiceTable[0].lpServiceProc = ServiceControlMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	if(StartServiceCtrlDispatcher(ServiceTable))
		return 0;

	return ERROR_INVALID_PARAMETER;
}
/*
 *	创建服务
 */
int ServiceCreate()
{
	CHAR				szBinaryPathName[MAX_PATH]	= {0}; 
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	
	::GetModuleFileName(NULL, szBinaryPathName, MAX_PATH);
	// 打开SC控制中心
	schSCManager = OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 
	if(NULL == schSCManager)
	{
		return GetLastError();
	}
	// 创建服务
	schService = CreateService( 
		schSCManager,				// SCManager database 
		str_ServiceName,            // name of service 
		str_ServiceDisplay,         // service name to display 
		SERVICE_ALL_ACCESS,			// desired access 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
		SERVICE_DEMAND_START,		// start type 
		SERVICE_ERROR_NORMAL,		// error control type 
		szBinaryPathName,			// service's binary 
		NULL,						// no load ordering group 
		NULL,						// no tag identifier 
		NULL,						// no dependencies 
		NULL,						// LocalSystem account 
		NULL);						// no password 
	if(NULL == schService)
	{
		CloseServiceHandle(schSCManager);
		return GetLastError();
	}
	// 更改服务选项
	bRes = ChangeServiceConfig(schService, 
		SERVICE_NO_CHANGE,			// Type of service
		SERVICE_AUTO_START,			// Service start options
		SERVICE_NO_CHANGE,			// Severity of the error
		NULL,						// path to the service binary file
		NULL,						// group of which this service is a member
		NULL,						// receives a tag value that is unique in the group specified in the lpLoadOrderGroup parameter
		NULL,						// system must start before this service can be started
		NULL,						// name of the account under which the service should run
		NULL,						// password to the account name specified by the lpServiceStartName parameter
		NULL						// display name to be used by applications 
		);

	SERVICE_DESCRIPTION			description;

	description.lpDescription = str_ServiceDescription;
	bRes = ChangeServiceConfig2(schService,
		SERVICE_CONFIG_DESCRIPTION,
		&description);

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
	return 0;
}

/*
 *	启动服务
 */
int ServiceStart()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet				= 0;

	// 打开SC控制中心
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return GetLastError();
	}
	// 打开服务
	schService = OpenService(schSCManager, str_ServiceName, SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		CloseServiceHandle(schSCManager);
		return GetLastError();
	}

	// 启动服务
	bRes = StartService(schService, NULL, NULL);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	return nRet;
}

/*
 *	服务入口函数
 */
void WINAPI	ServiceControlMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	int			nErr			= 0;

	g_sServiceStatus.dwServiceType = SERVICE_WIN32;
	g_sServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_sServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	g_sServiceStatus.dwWin32ExitCode = 0;
	g_sServiceStatus.dwServiceSpecificExitCode = 0;
	g_sServiceStatus.dwCheckPoint = 0;
	g_sServiceStatus.dwWaitHint = 0;

	g_hStatus = RegisterServiceCtrlHandler(str_ServiceName, ServiceControlHandler);
	if(g_hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}

	// 运行服务
	g_sServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_hStatus, &g_sServiceStatus);

	// 运行服务主体
	ServerMain(dwArgc, lpszArgv);

	// 退出服务
	g_sServiceStatus.dwWin32ExitCode = 0;
	g_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_hStatus, &g_sServiceStatus);
}

/*
 *	服务控制函数
 */
void WINAPI	ServiceControlHandler(DWORD fdwControl)
{
	switch(fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		g_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_PAUSE:
		g_sServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
	default:
		g_sServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	}

	SetServiceStatus(g_hStatus, &g_sServiceStatus);
}