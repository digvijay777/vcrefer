#pragma once
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
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

	if(FALSE == ::ShellExecuteEx(&sei))
		return 0; // �Թ���Ա�������ʧ��

	dwChildID = GetProcessId(sei.hProcess);
	CloseHandle(sei.hProcess);
	return dwChildID;
}