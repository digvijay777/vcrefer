#pragma once

typedef VOID (*RUNSERVER)(DWORD, CHAR*[]); 

class CServiceApp
{
public:
	CServiceApp(LPCTSTR lpSvcName);
	~CServiceApp();
private:
	TCHAR			m_szServiceName[MAX_PATH];
	TCHAR			m_szServiceDisplay[MAX_PATH];
	TCHAR			m_szServiceDescription[MAX_PATH];
	BOOL			m_bAutoStart;
	
	SC_HANDLE		m_schService;
	SC_HANDLE		m_schSCManager;

	BOOL	OpenSCManager();
	BOOL	OpenService();
public:
	void	SetServiceInfo(LPCTSTR lpSvcDisplay = NULL, LPCTSTR lpSvcDescription = NULL, BOOL bAutoStart = TRUE);
	BOOL	Create();
	BOOL	Delete();
	BOOL	Start();
	BOOL	Stop();
	BOOL	RunServer(RUNSERVER fnRunServer);
private:
	SERVICE_STATUS					m_sServiceStatus;
	SERVICE_STATUS_HANDLE			m_hStatus;
	RUNSERVER						m_fnRunServer;

public:
	static void WINAPI	ServiceControlMain(DWORD dwArgc, LPTSTR* lpszArgv);
	static void WINAPI	ServiceControlHandler(DWORD fdwControl);
	static CServiceApp*						s_pServiceApp;
};
