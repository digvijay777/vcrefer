/*
 * �����ģ�幤��
 * 
 * �������ʱ��: 2010-04-23
 * 
 * ʹ�÷���:		1) ����һ���࣬class CXXX : TServiceBase<CXXX>{}
 *				2) ����һ���麯��,  virtual void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
 *				3) CXXX����һ��ȫ�ֱ���
 *				4) ��main�����е���CXXX_var.ParseCommand(argc, argv),����TRUE��ʾ�������ദ��
					������û�����κδ�����Ҫ�Լ��ֶ�����
 *				5) IsServiceRun()���ڻص�����ServiceMain���жϷ����Ƿ�������
 *				6) ������ʹ�� "/create ":����, "/start":����, "/stop":ֹͣ, "/delete":ɾ��, "/debug":���� 
 *				7) �����ʹ��,��4)�еĲ������滹Ҫ���Ϸ����������û�з��������������κβ���
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
 *	���캯��
 *
 * ����˵��: lpServiceName������, lpServiceDisplay������ʾ��, lpServiceDescription ��������
 *          lpStartParam������������
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

// ��������
template <class T>
TServiceBase<T>::~TServiceBase()
{

}

// ��ʼ������
template <class T>
void TServiceBase<T>::ConfigService()
{
	m_dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	m_dwStartType = SERVICE_AUTO_START;
}

// ��������
template <class T>
BOOL TServiceBase<T>::ParseCommand(int argc, LPTSTR argv[])
{
	// ��֤����
	if("" != m_strStartParameter)
	{
		// �����������
		if((2 == argc) && argv[1] == m_strStartParameter)
		{
			StartServiceCtrl();
			return TRUE;
		}
		// ��������
		if((3 > argc) || (argv[2] != m_strStartParameter))
			return FALSE;
	}
	else if(1 == argc)
	{
		StartServiceCtrl();
		return TRUE;
	}
	// ��֤����
	if((2 > argc) || ((argv[1][0] != _T('-')) && (argv[1][0] != _T('/'))))
	{
		return FALSE;
	}

	LPCTSTR		lpOpt		= &argv[1][1];
	if(0 == _tcsicmp(lpOpt, _T("create"))) // ��������
	{
		ServiceCreate();
	}
	else if(0 == _tcsicmp(lpOpt, _T("start"))) // ��������
	{
		ServiceStart();
	}
	else if(0 == _tcsicmp(lpOpt, _T("stop"))) // ֹͣ����
	{
		ServiceStop();
	}
	else if(0 == _tcsicmp(lpOpt, _T("delete"))) // ɾ��
	{
		ServiceStop();
		ServiceDelete();
	}
#if defined(_DEBUG) || defined(DEBUG)
	else if(0 == _tcsicmp(lpOpt, _T("debug"))) // ��ʽ����,�����ڵ���ѡ���²���
	{
		m_bDebug = TRUE;
		ServiceMain(0, NULL);
	}
#endif
	else // ���Ǵ�����Խ����Ĳ���
	{
		return FALSE;
	}

	return TRUE;
}

// �������
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
		// д��־
		assert(false);
	}
}

// ������ƺ���
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

	// ���з���
	s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(s_pThis->s_hServiceHandle, &s_pThis->s_ServiceStatus);

	// ���з�������
	s_pThis->ServiceMain(dwArgc, lpszArgv);

	// �˳�����
	s_pThis->s_ServiceStatus.dwWin32ExitCode = 0;
	s_pThis->s_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(s_pThis->s_hServiceHandle, &s_pThis->s_ServiceStatus);
}

// ������ƺ���
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

// ��������
template <class T>
BOOL TServiceBase<T>::ServiceCreate()
{
	TCHAR				szBinaryPathName[MAX_PATH*2]	= {0}; 
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet							= 0;

	::GetModuleFileName(NULL, szBinaryPathName, MAX_PATH*2);
	// ��SC��������
	schSCManager = ::OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// �����������
	if("" != m_strStartParameter)
	{
		_tcscat(szBinaryPathName, " ");
		_tcscat(szBinaryPathName, m_strStartParameter.c_str());
	}
	// ��������
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
	// ���ķ���ѡ��
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

// ��������
template <class T>
BOOL TServiceBase<T>::ServiceStart()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet				= 0;

	// ��SC��������
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// �򿪷���
	schService = OpenService(schSCManager, m_strServiceName.c_str(), SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		nRet = GetLastError();
		CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}

	// ��������
	bRes = StartService(schService, NULL, NULL);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return bRes;
}

// ֹͣ����
template <class T>
BOOL TServiceBase<T>::ServiceStop()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	SERVICE_STATUS		ServiceStatus;
	BOOL				bRes;
	int					nRet				= 0;

	// ��SC��������
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// �򿪷���
	schService = OpenService(schSCManager, m_strServiceName.c_str(), SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		nRet = GetLastError();
		CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}

	// ֹͣ����
	QueryServiceStatus(schService, &ServiceStatus);
	bRes = ControlService(schService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return TRUE;
}

// ɾ������
template <class T>
BOOL TServiceBase<T>::ServiceDelete()
{
	SC_HANDLE			schService;
	SC_HANDLE			schSCManager;
	BOOL				bRes;
	int					nRet				= 0;

	// ��SC��������
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == schSCManager)
	{
		return FALSE;
	}
	// �򿪷���
	schService = OpenService(schSCManager, m_strServiceName.c_str(), SC_MANAGER_ALL_ACCESS);
	if(NULL == schService)
	{
		nRet = GetLastError();
		CloseServiceHandle(schSCManager);
		SetLastError(nRet);
		return FALSE;
	}

	// ֹͣ����
	bRes = DeleteService(schService);
	if(FALSE == bRes)
		nRet = GetLastError();

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	SetLastError(nRet);
	return TRUE;
}

// �����Ƿ�������
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