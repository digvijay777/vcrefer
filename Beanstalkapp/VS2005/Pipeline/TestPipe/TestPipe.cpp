// TestPipe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>


int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO					si				= {0};
	STARTUPINFO					siold			= {0};
	PROCESS_INFORMATION			pi				= {0};
	CHAR						szTest[1024]	= {"这里是一个PIPE的测试"};
	DWORD						dwWrite			= 0;
	SECURITY_ATTRIBUTES			sa				= {0};
	HANDLE						hWrite			= 0;

	sa.nLength              = sizeof(sa);
	sa.bInheritHandle       = TRUE;			//	CreateProcessA 继承时需要也 继承
	sa.lpSecurityDescriptor = NULL;

	GetStartupInfo(&siold);

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	if(FALSE == CreatePipe(&si.hStdInput, &hWrite, &sa, 1024))
	{
		printf("创建PIPE失败\n");
		return 0;
	}
	WriteFile(hWrite, szTest, (DWORD)strlen(szTest)+1, &dwWrite, NULL);
	CloseHandle(hWrite);

	if(FALSE == CreateProcess("InPipe.exe", NULL, NULL, NULL
		, TRUE, 0, NULL, NULL, &si, &pi))
	{
		printf("创建进程失败: %d\n", GetLastError());
		goto end;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
end:
	CloseHandle(si.hStdInput);
	//CloseHandle(si.hStdInput);

	return 0;
}

