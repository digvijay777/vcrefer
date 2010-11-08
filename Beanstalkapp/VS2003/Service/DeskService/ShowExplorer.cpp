#include "stdafx.h"
#include "control.h"


/*
 *	œ‘ æ◊¿√Ê
 */
int ShowExplorer()
{
	MessageBox(NULL, "This is test.", "Explorer", MB_OK | MB_ICONINFORMATION);
	//MessageBeep(MB_ICONEXCLAMATION);

	STARTUPINFO				si						= {0};
	PROCESS_INFORMATION		pi						= {0};
	CHAR					szExplorer[MAX_PATH]	= {0};

	GetWindowsDirectory(szExplorer, MAX_PATH);
	strcat(szExplorer, "\\explorer.exe");
	if(CreateProcess(szExplorer, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}