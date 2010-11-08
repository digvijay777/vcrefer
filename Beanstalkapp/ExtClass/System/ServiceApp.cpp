#include "stdafx.h"
#include "ServiceApp.h"

#ifndef TRACELOG
#include <Atlbase.h>
#define TRACELOG		ATLTRACE
#endif

#if !defined(str_ServiceName) || !defined(str_ServiceDisplay) || !defined(str_ServiceDescription)
#error "Can not define service info"
#endif

CServiceApp*						CServiceApp::s_pServiceApp			= NULL;

CServiceApp::CServiceApp(LPCSTR lpSvcName)
{
	memset(m_szServiceName, 0, sizeof(m_szServiceName) * sizeof(TCHAR));
	memset(m_szServiceDisplay, 0, sizeof(m_szServiceDisplay) * sizeof(TCHAR));
	memset(m_szServiceDescription, 0, sizeof(m_szServiceDescription) * sizeof(TCHAR));

	_tcscpy(m_szServiceName, lpSvcName);
	m_bAutoStart = TRUE;
	m_schService = NULL;
	m_schSCManager = NULL;
	s_pServiceApp = this;
}

CServiceApp::~CServiceApp()
{
	if(NULL != m_schService)
		CloseServiceHandle(m_schService);
	if(NULL != m_schSCManager)
		CloseServiceHandle(m_schSCManager);
}

/*
 *	设置服务信息
 */
void CServiceApp::SetServiceInfo(LPCTSTR lpSvcDisplay /* = NULL */, LPCTSTR lpSvcDescription /* = NULL */, BOOL bAutoStart /* = TRUE */)
{
	if(NULL != lpSvcDisplay)
		_tcscpy(m_szServiceDisplay, lpSvcDisplay);
	if(NULL != lpSvcDescription)
		_tcscpy(m_szServiceDescription, lpSvcDescription);
	m_bAutoStart = bAutoStart;
}

/*
 *	打开服务控制中心
 */
BOOL CServiceApp::OpenSCManager()
{
	if(NULL != m_schSCManager)
		return TRUE;

	if(NULL == m_schSCManager)
	{
		m_schSCManager = ::OpenSCManager( 
			NULL,                    // local machine 
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 
	}
	if(NULL == m_schSCManager)
	{
		TRACELOG("Open SC Manager center failed: %d\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}
/*
 *	打开服务
 */
BOOL CServiceApp::OpenService()
{
	if(NULL != m_schService)
		return TRUE;

	if(FALSE == OpenSCManager())
        return FALSE;
	
	// 打开服务
	m_schService = ::OpenService(m_schSCManager, m_szServiceName, SC_MANAGER_ALL_ACCESS);
	if(NULL == m_schService)
	{
		TRACELOG("Open Service %s failed: %d\n", m_szServiceName, GetLastError());
		return FALSE;
	}

	return TRUE;
}

/*
 *	创建服务
 */
BOOL CServiceApp::Create()
{
	if(FALSE == OpenSCManager())
		return FALSE;

	CHAR				szBinaryPathName[MAX_PATH]	= {0};
	BOOL				bRes						= FALSE;
	DWORD				dwStartMode					= SERVICE_DEMAND_START;

	::GetModuleFileName(NULL, szBinaryPathName, MAX_PATH);
	if(FALSE != m_bAutoStart)
		dwStartMode = SERVICE_AUTO_START;
	// 创建服务
	m_schService = CreateService( 
		m_schSCManager,				// SCManager database 
		m_szServiceName,            // name of service 
		m_szServiceDisplay,         // service name to display 
		SERVICE_ALL_ACCESS,			// desired access 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
		dwStartMode,				// start type 
		SERVICE_ERROR_NORMAL,		// error control type 
		szBinaryPathName,			// service's binary 
		NULL,						// no load ordering group 
		NULL,						// no tag identifier 
		NULL,						// no dependencies 
		NULL,						// LocalSystem account 
		NULL);						// no password 
	if(NULL == m_schService)
	{
		TRACELOG("Create Service %s failed: %d\n", m_szServiceName, GetLastError());
		return FALSE;
	}

	SERVICE_DESCRIPTION			description;

	description.lpDescription = m_szServiceDescription;
	bRes = ChangeServiceConfig2(m_schService,
		SERVICE_CONFIG_DESCRIPTION,
		&description);

	return TRUE;
}

/*
 *	启动服务
 */
BOOL CServiceApp::Start()
{
	if(FALSE == OpenService())
		return FALSE;

	BOOL			bRes		= FALSE;

	bRes = StartService(m_schService, NULL, NULL);
	if(FALSE == bRes)
	{
		TRACELOG("Start service %s failed: %d\n", m_szServiceName, GetLastError());
	}

	return bRes;
}

/*
 *	停止服务
 */
BOOL CServiceApp::Stop()
{
	if(FALSE == OpenService())
		return FALSE;

	BOOL				bRes		= FALSE;
	SERVICE_STATUS		SvcStatus;

	bRes = ControlService(m_schService, SERVICE_CONTROL_STOP, &SvcStatus);
	if(FALSE == bRes)
	{
		TRACELOG("Stop service %s failed: %d\n", m_szServiceName, GetLastError());
	}

	return bRes;
}
/*
 *	删除服务
 */
BOOL CServiceApp::Delete()
{
	if(FALSE == OpenService())
		return FALSE;

	BOOL				bRes		= FALSE;

	bRes = DeleteService(m_schService);
	if(FALSE == bRes)
	{
		TRACELOG("Delete service %s failed: %d\n", m_szServiceName, GetLastError());
	}

	return bRes;
}

/*
 *	启动服启控制函数
 */
BOOL CServiceApp::RunServer(RUNSERVER fnRunServer)
{
	SERVICE_TABLE_ENTRY		ServiceTable[2]		= {0};

	ServiceTable[0].lpServiceName = m_szServiceName;
	ServiceTable[0].lpServiceProc = ServiceControlMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	m_fnRunServer = fnRunServer;
	return StartServiceCtrlDispatcher(ServiceTable);
}

void WINAPI	CServiceApp::ServiceControlMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	int			nErr			= 0;

	s_pServiceApp->m_sServiceStatus.dwServiceType = SERVICE_WIN32;
	s_pServiceApp->m_sServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	s_pServiceApp->m_sServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	s_pServiceApp->m_sServiceStatus.dwWin32ExitCode = 0;
	s_pServiceApp->m_sServiceStatus.dwServiceSpecificExitCode = 0;
	s_pServiceApp->m_sServiceStatus.dwCheckPoint = 0;
	s_pServiceApp->m_sServiceStatus.dwWaitHint = 0;

	s_pServiceApp->m_hStatus = RegisterServiceCtrlHandler(s_pServiceApp->m_szServiceName, ServiceControlHandler);
	if(s_pServiceApp->m_hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}

	// 运行服务
	s_pServiceApp->m_sServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(s_pServiceApp->m_hStatus, &s_pServiceApp->m_sServiceStatus);

	// 运行服务主体
	if(NULL != s_pServiceApp->m_fnRunServer)
		s_pServiceApp->m_fnRunServer(dwArgc, lpszArgv);

	// 退出服务
	s_pServiceApp->m_sServiceStatus.dwWin32ExitCode = 0;
	s_pServiceApp->m_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(s_pServiceApp->m_hStatus, &s_pServiceApp->m_sServiceStatus);
}

void WINAPI	CServiceApp::ServiceControlHandler(DWORD fdwControl)
{
	switch(fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		s_pServiceApp->m_sServiceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_PAUSE:
		s_pServiceApp->m_sServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
	default:
		s_pServiceApp->m_sServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	}

	SetServiceStatus(s_pServiceApp->m_hStatus, &s_pServiceApp->m_sServiceStatus);
}