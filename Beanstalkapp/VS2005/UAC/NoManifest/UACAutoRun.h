#pragma once
#include <shlobj.h>
#include <shlwapi.h>
#include <Tlhelp32.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")

//////////////////////////////////////////////////////////////////////////
// Windows vista ����
// typedef struct _PROC_THREAD_ATTRIBUTE_LIST *PPROC_THREAD_ATTRIBUTE_LIST, *LPPROC_THREAD_ATTRIBUTE_LIST;
typedef void * LPPROC_THREAD_ATTRIBUTE_LIST, *PPROC_THREAD_ATTRIBUTE_LIST;

typedef struct _STARTUPINFOEXA {
	STARTUPINFOA StartupInfo;
	LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
} STARTUPINFOEXA, *LPSTARTUPINFOEXA;
typedef struct _STARTUPINFOEXW {
	STARTUPINFOW StartupInfo;
	LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
} STARTUPINFOEXW, *LPSTARTUPINFOEXW;
#ifdef UNICODE
typedef STARTUPINFOEXW STARTUPINFOEX;
typedef LPSTARTUPINFOEXW LPSTARTUPINFOEX;
#else
typedef STARTUPINFOEXA STARTUPINFOEX;
typedef LPSTARTUPINFOEXA LPSTARTUPINFOEX;
#endif // UNICODE


#define EXTENDED_STARTUPINFO_PRESENT			0x00080000



#define PROC_THREAD_ATTRIBUTE_NUMBER    0x0000FFFF
#define PROC_THREAD_ATTRIBUTE_THREAD    0x00010000  // Attribute may be used with thread creation
#define PROC_THREAD_ATTRIBUTE_INPUT     0x00020000  // Attribute is input only
#define PROC_THREAD_ATTRIBUTE_ADDITIVE  0x00040000  // Attribute may be "accumulated," e.g. bitmasks, counters, etc.

typedef enum _PROC_THREAD_ATTRIBUTE_NUM {
	ProcThreadAttributeParentProcess = 0,
	ProcThreadAttributeExtendedFlags,
	ProcThreadAttributeHandleList,
	ProcThreadAttributeGroupAffinity,
	ProcThreadAttributePreferredNode,
	ProcThreadAttributeIdealProcessor,
	ProcThreadAttributeUmsThread,
	ProcThreadAttributeMitigationPolicy,
	ProcThreadAttributeMax
} PROC_THREAD_ATTRIBUTE_NUM;

#define ProcThreadAttributeValue(Number, Thread, Input, Additive) \
	(((Number) & PROC_THREAD_ATTRIBUTE_NUMBER) | \
	((Thread != FALSE) ? PROC_THREAD_ATTRIBUTE_THREAD : 0) | \
	((Input != FALSE) ? PROC_THREAD_ATTRIBUTE_INPUT : 0) | \
	((Additive != FALSE) ? PROC_THREAD_ATTRIBUTE_ADDITIVE : 0))

#define PROC_THREAD_ATTRIBUTE_PARENT_PROCESS \
	ProcThreadAttributeValue (ProcThreadAttributeParentProcess, FALSE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_EXTENDED_FLAGS \
	ProcThreadAttributeValue (ProcThreadAttributeExtendedFlags, FALSE, TRUE, TRUE)
#define PROC_THREAD_ATTRIBUTE_HANDLE_LIST \
	ProcThreadAttributeValue (ProcThreadAttributeHandleList, FALSE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_GROUP_AFFINITY \
	ProcThreadAttributeValue (ProcThreadAttributeGroupAffinity, TRUE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_PREFERRED_NODE \
	ProcThreadAttributeValue (ProcThreadAttributePreferredNode, FALSE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_IDEAL_PROCESSOR \
	ProcThreadAttributeValue (ProcThreadAttributeIdealProcessor, TRUE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_UMS_THREAD \
	ProcThreadAttributeValue (ProcThreadAttributeUmsThread, TRUE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY \
	ProcThreadAttributeValue (ProcThreadAttributeMitigationPolicy, FALSE, TRUE, FALSE)

//////////////////////////////////////////////////////////////////////////
/*
 *	��������Ȩ��
 */
BOOL EnablePrivilege( LPCTSTR name )  
{ 
	BOOL					bRet				=	FALSE;  
	TOKEN_PRIVILEGES		priv				= { 1, { 0, 0, SE_PRIVILEGE_ENABLED } };  
	BOOL					bLookup				= LookupPrivilegeValue( NULL, name, &priv.Privileges[0].Luid );  

	if( !bLookup )  
	{  
		//OutLog("����ָ��Ȩ��ֵʧ��");  
		return bRet;  
	}  
	//�򿪽��̵ķ��ʱ��  
	HANDLE hToken = NULL;  
	BOOL bOpenToken = OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );  
	if( !bOpenToken )  
	{  
		//OutLog("�򿪽��̵ķ��ʱ��ʧ��");  
		return bRet;  
	}  

	//����Ȩ��  
	BOOL bAdjustpriv = AdjustTokenPrivileges( hToken, false, &priv, sizeof( priv ), 0, 0 );  
	if( !bAdjustpriv )  
	{  
		//OutLog("����Ȩ��ʧ��");  
		CloseHandle( hToken );  
		return bRet;  
	}  
	CloseHandle( hToken );  

	return TRUE;  
}  

/*
 *	�õ��û�Ȩ��
 */
BOOL GetTokenByProcName(HANDLE &hToken,	LPCTSTR lpName)
{
	if(!lpName)
		return FALSE;

	HANDLE				hProcessSnap		= NULL;  
	BOOL				bRet				= FALSE;  
	PROCESSENTRY32		pe32				= {0};  

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if (hProcessSnap == INVALID_HANDLE_VALUE)  
		return FALSE;  

	pe32.dwSize = sizeof(PROCESSENTRY32);  
	if (Process32First(hProcessSnap, &pe32))  
	{
		do  
		{
			if(0 == _tcsicmp(pe32.szExeFile, lpName))
			{
				hToken = OpenProcess(MAXIMUM_ALLOWED, FALSE,pe32.th32ProcessID);
				if(NULL != hToken)
				{
					bRet = TRUE;
					break;
				}
			}
		}  
		while (Process32Next(hProcessSnap, &pe32));  
	}  
	else  
	{
		bRet = FALSE;
	}

	CloseHandle (hProcessSnap);  
	return (bRet);
}
/*
 *	����UACȨ������
 */
BOOL RunSkipUAC(LPCTSTR lpExe, LPCTSTR lpCmd)
{
	STARTUPINFOEX					siex			= {0};
	PROCESS_INFORMATION				pi				= {0};
	HINSTANCE						hInstKel32		= LoadLibrary(_T("Kernel32.dll"));
	SIZE_T							cbAListSize		= 0;
	PPROC_THREAD_ATTRIBUTE_LIST		pAList			= NULL;
	HANDLE							hToken			= NULL;
	HANDLE							hUser			= NULL;
	BOOL							bRet			= FALSE;

	typedef BOOL (WINAPI *fnInitializeProcThreadAttributeList)(LPPROC_THREAD_ATTRIBUTE_LIST 
		, DWORD, DWORD, PSIZE_T);
	typedef BOOL (WINAPI *fnUpdateProcThreadAttribute)(LPPROC_THREAD_ATTRIBUTE_LIST
		, DWORD, DWORD_PTR, PVOID, SIZE_T, PVOID, PSIZE_T);
	typedef VOID (WINAPI *fnDeleteProcThreadAttributeList)(LPPROC_THREAD_ATTRIBUTE_LIST);

	fnInitializeProcThreadAttributeList				InitializeProcThreadAttributeList			= NULL;
	fnUpdateProcThreadAttribute						UpdateProcThreadAttribute					= NULL;
	fnDeleteProcThreadAttributeList					DeleteProcThreadAttributeList				= NULL;

	InitializeProcThreadAttributeList = (fnInitializeProcThreadAttributeList)GetProcAddress(hInstKel32
		, "InitializeProcThreadAttributeList");
	UpdateProcThreadAttribute = (fnUpdateProcThreadAttribute)GetProcAddress(hInstKel32
		, "UpdateProcThreadAttribute");
	DeleteProcThreadAttributeList = (fnDeleteProcThreadAttributeList)GetProcAddress(hInstKel32
		, "DeleteProcThreadAttributeList");
	if(NULL == InitializeProcThreadAttributeList
		|| NULL == UpdateProcThreadAttribute
		|| NULL == DeleteProcThreadAttributeList)
	{
		return FALSE;
	}
	// ��ʼ��Ȩ����
	EnablePrivilege(SE_SECURITY_NAME);

	siex.StartupInfo.cb = sizeof(STARTUPINFOEX);
	if(FALSE == GetTokenByProcName(hToken, _T("explorer.exe")))
		return FALSE;
	DuplicateTokenEx(hToken, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY
		, NULL, SecurityImpersonation, TokenPrimary, &hUser);
	// ��ʼ��
	InitializeProcThreadAttributeList(NULL, 1, 0, &cbAListSize);
	if(0 == cbAListSize)
		return FALSE;
	// ����ռ�
	pAList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, cbAListSize);
	if(NULL == pAList)
		return FALSE;
	// ����Ȩ��
	InitializeProcThreadAttributeList(pAList, 1, 0, &cbAListSize);
	UpdateProcThreadAttribute(pAList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hToken
		, sizeof(HANDLE), NULL, NULL);
	siex.lpAttributeList = pAList;
	if(CreateProcess(lpExe, (LPTSTR)lpCmd, NULL, NULL, FALSE
		, EXTENDED_STARTUPINFO_PRESENT
		, NULL, NULL, &siex.StartupInfo, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		bRet = TRUE;
	}
	DeleteProcThreadAttributeList(pAList);
	if(NULL != pAList)
		HeapFree(GetProcessHeap(), 0, pAList);
	return bRet;
}
/*
 *	UAC��Ȩ����
 *  ����ֵ: 0  ��ʾ�Ѿ����Ȩ��
 *         ��0 ��ʾ��Ȩ������һ���µĳ���
 */
DWORD RunAsUAC()
{
	OSVERSIONINFO			ovi		= {0};

	ovi.dwOSVersionInfoSize = sizeof(ovi);
	if(::GetVersionEx(&ovi))
	{
			// Windows �汾С��Vista
		if(ovi.dwMajorVersion < 6)
			return 0;
	}
	// Windows Vista�����ϰ汾
	DWORD			dwType;
	DWORD			dwValue		= 0;
	DWORD			dwSize		= sizeof(dwValue);

	SHGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\")
		, _T("EnableLUA"), &dwType, &dwValue, &dwSize);
	if(0 == dwValue)
		return 0;	// û��UACȨ�޿���
	// �ӳ�������
	if(IsUserAnAdmin())
		return 0;
	// UAC��Ȩ����
	LPCTSTR				lpCommandLine			= NULL;
	SHELLEXECUTEINFO	sei						= {0};
	STARTUPINFO			si						= {0};
	TCHAR				szImageFile[MAX_PATH]	= {0};
	DWORD				dwChildID				= 0;

	// �õ�����
	lpCommandLine = ::GetCommandLine();
	if(lpCommandLine[0] == L'\"')
	{
		do
		{
			lpCommandLine++;
		}while(lpCommandLine[0] != L'\"');
		lpCommandLine++;//pass the last quotes

		if(lpCommandLine[0] == L' ')//����в���,��ô������и��ո�
		{
			lpCommandLine++;
		}
	}
	else
	{
		while(lpCommandLine[0] != L' ')// space
		{
			lpCommandLine++;
		}
		lpCommandLine++;//pass the last space
	}
	// ����������
	GetStartupInfo(&si);
	GetModuleFileName(NULL, szImageFile, MAX_PATH);

	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.nShow = si.wShowWindow;
	sei.lpFile = szImageFile;
	sei.lpVerb = _T("runas");
	sei.lpParameters = lpCommandLine;
	sei.lpDirectory = NULL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;

// 	if(FALSE == RunSkipUAC(_T("test.exe")/*szImageFile*/, lpCommandLine))
// 	{
		if(FALSE == ::ShellExecuteEx(&sei))
			return 0; // �Թ���Ա�������ʧ��
// 	}

	dwChildID = GetProcessId(sei.hProcess);
	CloseHandle(sei.hProcess);
	return dwChildID;
}