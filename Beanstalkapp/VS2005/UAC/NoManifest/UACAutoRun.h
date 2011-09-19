#pragma once
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
/*
 *	UAC提权运行
 *  返回值: 0  表示已经提过权限
 *         非0 表示提权运行了一个新的程序
 */
DWORD RunAsUAC()
{
	OSVERSIONINFO			ovi		= {0};

	ovi.dwOSVersionInfoSize = sizeof(ovi);
	if(::GetVersionEx(&ovi))
	{
			// Windows 版本小于Vista
		if(ovi.dwMajorVersion < 6)
			return 0;
	}
	// Windows Vista及以上版本
	DWORD			dwType;
	DWORD			dwValue		= 0;
	DWORD			dwSize		= sizeof(dwValue);

	SHGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\")
		, _T("EnableLUA"), &dwType, &dwValue, &dwSize);
	if(0 == dwValue)
		return 0;	// 没有UAC权限控制
	// 子程序运行
	if(IsUserAnAdmin())
		return 0;
	// UAC提权运行
	LPCTSTR				lpCommandLine			= NULL;
	SHELLEXECUTEINFO	sei						= {0};
	STARTUPINFO			si						= {0};
	TCHAR				szImageFile[MAX_PATH]	= {0};
	DWORD				dwChildID				= 0;

	// 得到参数
	lpCommandLine = ::GetCommandLine();
	if(lpCommandLine[0] == L'\"')
	{
		do
		{
			lpCommandLine++;
		}while(lpCommandLine[0] != L'\"');
		lpCommandLine++;//pass the last quotes

		if(lpCommandLine[0] == L' ')//如果有参数,那么这里会有个空格
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
	// 构建启动项
	GetStartupInfo(&si);
	GetModuleFileName(NULL, szImageFile, MAX_PATH);

	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.nShow = si.wShowWindow;
	sei.lpFile = szImageFile;
	sei.lpVerb = _T("runas");
	sei.lpParameters = lpCommandLine;
	sei.lpDirectory = NULL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;

	if(FALSE == ::ShellExecuteEx(&sei))
		return 0; // 以管理员身份运行失败

	dwChildID = GetProcessId(sei.hProcess);
	CloseHandle(sei.hProcess);
	return dwChildID;
}