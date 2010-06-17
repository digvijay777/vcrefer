/*
 * 多服务模板工程
 * 
 * 初次完成时间: 2010-04-23
 * 
 * 使用方法:		1) 定义一个类，class CXXX : TServiceBase<CXXX>{}
 *				2) 申名一个虚函数,  virtual void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
 *				3) CXXX申名一个全局变量
 *				4) 在main函数中调用CXXX_var.ParseCommand(argc, argv),返回TRUE表示被服务类处理，
					否则是没有做任何处理需要自己手动处理
 *				5) IsServiceRun()是在回调函数ServiceMain中判断服务是否还在运行
 *				6) 单服务使用 "/create ":创建, "/start":启动, "/stop":停止, "/delete":删除, "/debug":调试 
 *				7) 多服务使用,在4)中的参数后面还要加上服务名，如果没有服务名，将不做任何操作
 */
#pragma once
#include <string>
#include <assert.h>

#if defined(UNICODE) || defined(_UNICODE)
	#define String			std::wstring
#else
	#define String			std::string
#endif

template <class T>
class TServiceBase 
{
public:
	TServiceBase(LPCTSTR lpServiceName, LPCTSTR lpServiceDisplay, LPCTSTR lpServiceDescription, LPCTSTR lpStartParam = NULL);
	virtual ~TServiceBase();
protected:
	String		m_strServiceName;
	String		m_strServiceDisplay;
	String		m_strServiceDescription;
	String		m_strStartParameter;

	DWORD		m_dwServiceType;
	DWORD		m_dwStartType;
	BOOL		m_bDebug;
private:
	static TServiceBase<T>*		s_pThis;
	SERVICE_STATUS				s_ServiceStatus;
	SERVICE_STATUS_HANDLE		s_hServiceHandle;

protected:
	void		StartServiceCtrl();
	BOOL		IsServiceRun();
private:
	BOOL		ServiceCreate();
	BOOL		ServiceStart();
	BOOL		ServiceStop();
	BOOL		ServiceDelete();

	static void	WINAPI _ServiceControlProc(DWORD dwArgc, LPTSTR* lpszArgv);
	static void WINAPI _ServiceControlHandler(DWORD fdwControl);
public:
	virtual BOOL ParseCommand(int argc, LPTSTR argv[]);
	virtual void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) = 0;
	virtual void ConfigService();
};

template <class T>
TServiceBase<T>*			TServiceBase<T>::s_pThis				= NULL;

/*
 *	构造函数
 *
 * 参数说明: lpServiceName服务名, lpServiceDisplay服务显示名, lpServiceDescription 服务描述
 *          lpStartParam服务启动参数
 */ 
template <class T>
TServiceBase<T>::TServiceBase(LPCTSTR lpServiceName, LPCTSTR lpServiceDisplay, 
							  LPCTSTR lpServiceDescription, LPCTSTR lpStartParam /* = NULL */)
{
	assert(NULL != lpServiceName);
	assert(NULL != lpServiceName);
	assert(NULL != lpServiceDisplay);
	assert(NULL != lpServiceDescription);

	m_strServiceName = lpServiceName;
	m_strServiceDisplay = lpServiceDisplay;
	m_strServiceDescription = lpServiceDescription;
	if(NULL != lpStartParam)
		m_strStartParameter = lpStartParam;
	m_bDebug = FALSE;

	assert(NULL == s_pThis);
	s_pThis = this;

	ConfigService();
}

// 析构函数
template <class T>
TServiceBase<T>::~TServiceBase()
{

}

// 初始化服务
template <class T>
void TServiceBase<T>::ConfigService()
{
	m_dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	m_dwStartType = SERVICE_AUTO_START;
}

// 解析参数
template <class T>
BOOL TServiceBase<T>::ParseCommand(int argc, LPTSTR argv[])
{
	// 验证服务
	if("" != m_strStartParameter)
	{
		// 启动服务代码
		if((2 == argc) && argv[1] == m_strStartParameter)
		{
			StartServiceCtrl();
			return TRUE;
		}
		// 其它操作
		if((3 > argc) || (argv[2] != m_strStartParameter))
			return FALSE;
	}
	else if(1 == argc)
	{
		StartServiceCtrl();
		return TRUE;
	}
	// 验证参数
	if((2 > argc) || ((argv[1][0] != _T('-')) && (argv[1][0] != _T('/'))))
	{
		return FALSE;
	}

	LPCTSTR		lpOpt		= &argv[1][1];
	if(0 == _tcsicmp(lpOpt, _T("create"))) // 创建服务
	{
		ServiceCreate();
	}
	else if(0 == _tcsicmp(lpOpt, _T("start"))) // 启动服务
	{
		ServiceStart();
	}
	else if(0 == _tcsicmp(lpOpt, _T("stop"))) // 停止服务
	{
		ServiceStop();
	}
	else if(0 == _tcsicmp(lpOpt, _T("delete"))) // 删除
	{
		ServiceStop();
		ServiceDelete();
	}
#if defined(_DEBUG) || defined(DEBUG)
	else if(0 == _tcsicmp(lpOpt, _T("debug"))) // 调式服务,仅仅在调试选项下才有
	{
		m_bDebug = TRUE;
		ServiceMain(0, NULL);
	}
#endif
	else // 不是此类可以解析的参数
	{
		return FALSE;
	}

	return TRUE;
}

// 服务控制
template <class T>
void TServiceBase<T>::StartServiceCtrl()
{
	SERVICE_TABLE_ENTRY		ServiceTable[2]		= {0};

	ServiceTable[0].lpServiceName = (LPSTR)m_strServiceName.c_str();
	ServiceTable[0].lpServiceProc = TServiceBase<T>::_ServiceControlProc;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	if(!::StartServiceCtrlDispatcher(ServiceTable))
	{
		// 写日志
		assert(false);
	}
}

// 服务控制函数
template <class T>
void WINAPI TServiceBase<T>::_ServiceControlProc(DWORD dwArgc, LPTSTR* lpszArgv)
{
	assert(NULL != s_pThis);

	int			nErr			= 0;

	s_pThis->s_ServiceStatus.dwServiceType = SERVICE_WIN32;
	s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	s_pThis->s_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	s_pThis->s_ServiceStatus.dwWin32ExitCode = 0;
	s_pThis->s_ServiceStatus.dwServiceSpecificExitCode = 0;
	s_pThis->s_ServiceStatus.dwCheckPoint = 0;
	s_pThis->s_ServiceStatus.dwWaitHint = 0;

	s_pThis->s_hServiceHandle = RegisterServiceCtrlHandler(s_pThis->m_strServiceName.c_str(), _ServiceControlHandler);
	if((SERVICE_STATUS_HANDLE)0 == s_pThis->s_hServiceHandle)
	{
		assert(false);
		return;
	}

	// 运行服务
	s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(s_pThis->s_hServiceHandle, &s_pThis->s_ServiceStatus);

	// 运行服务主体
	s_pThis->ServiceMain(dwArgc, lpszArgv);

	// 退出服务
	s_pThis->s_ServiceStatus.dwWin32ExitCode = 0;
	s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(s_pThis->s_hServiceHandle, &s_pThis->s_ServiceStatus);
}

// 服务控制函数
template <class T>
void WINAPI TServiceBase<T>::_ServiceControlHandler(DWORD fdwControl)
{
	assert(NULL != s_pThis);

	switch(fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_PAUSE:
		s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
	default:
		s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	}

	SetServiceStatus(s_pThis->s_hServiceHandle, &s_pThis->s_ServiceStatus);
}

// 创建服务
template <class T>
BOOL TServiceBase<T>::ServiceCreate()
{
	TCHAR				szBinaryPathName[MAX_PATH*2]	= {0}; 
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet							= 0;

	::GetModuleFileName(NULL, szBinaryPathName, MAX_PATH*2);
	// 打开SC控制中心
	schSCManager = ::OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// 添加启动参数
	if("" != m_strStartParameter)
	{
		_tcscat(szBinaryPathName, " ");
		_tcscat(szBinaryPathName, m_strStartParameter.c_str());
	}
	// 创建服务
	schService = ::CreateService( 
		schSCManager,				// SCManager database 
		m_strServiceName.c_str(),            // name of service 
		m_strServiceDisplay.c_str(),         // service name to display 
		SERVICE_ALL_ACCESS,			// desired access 
		m_dwServiceType,			// service type 
		m_dwStartType,				// start type 
		SERVICE_ERROR_NORMAL,		// error control type 
		szBinaryPathName,			// service's binary 
		NULL,						// no load ordering group 
		NULL,						// no tag identifier 
		NULL,						// no dependencies 
		NULL,						// LocalSystem account 
		NULL);						// no password 
	if(NULL == schService)
	{
		nRet = GetLastError();
		::CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}
	// 更改服务选项
// 	bRes = ChangeServiceConfig(schService, 
// 		SERVICE_NO_CHANGE,			// Type of service
// 		m_dwStartType,				// Service start options
// 		SERVICE_NO_CHANGE,			// Severity of the error
// 		NULL,						// path to the service binary file
// 		NULL,						// group of which this service is a member
// 		NULL,						// receives a tag value that is unique in the group specified in the lpLoadOrderGroup parameter
// 		NULL,						// system must start before this service can be started
// 		NULL,						// name of the account under which the service should run
// 		NULL,						// password to the account name specified by the lpServiceStartName parameter
// 		NULL						// display name to be used by applications 
// 		);

	SERVICE_DESCRIPTION			description;

	description.lpDescription = (TCHAR *)m_strServiceDescription.c_str();
	bRes = ::ChangeServiceConfig2(schService,
		SERVICE_CONFIG_DESCRIPTION,
		&description);
	nRet = GetLastError();

	::CloseServiceHandle(schService); 
	::CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return bRes;
}

// 启动服务
template <class T>
BOOL TServiceBase<T>::ServiceStart()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet				= 0;

	// 打开SC控制中心
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// 打开服务
	schService = OpenService(schSCManager, m_strServiceName.c_str(), SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		nRet = GetLastError();
		CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}

	// 启动服务
	bRes = StartService(schService, NULL, NULL);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return bRes;
}

// 停止服务
template <class T>
BOOL TServiceBase<T>::ServiceStop()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	SERVICE_STATUS		ServiceStatus;
	BOOL				bRes;
	int					nRet				= 0;

	// 打开SC控制中心
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// 打开服务
	schService = OpenService(schSCManager, m_strServiceName.c_str(), SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		nRet = GetLastError();
		CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}

	// 停止服务
	QueryServiceStatus(schService, &ServiceStatus);
	bRes = ControlService(schService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return TRUE;
}

// 删除服务
template <class T>
BOOL TServiceBase<T>::ServiceDelete()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet				= 0;

	// 打开SC控制中心
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// 打开服务
	schService = OpenService(schSCManager, m_strServiceName.c_str(), SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		nRet = GetLastError();
		CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}

	// 停止服务
	bRes = DeleteService(schService);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return TRUE;
}

// 服务是否在运行
template <class T>
BOOL TServiceBase<T>::IsServiceRun()
{
	if(FALSE != m_bDebug)
		return TRUE;

	if(NULL == s_hServiceHandle)
	{
		assert(false);
		return FALSE;
	}

	if(SERVICE_STOPPED == s_ServiceStatus.dwCurrentState)
		return FALSE;

	return TRUE;
}