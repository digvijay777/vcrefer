// PipeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>

int _tmain(int argc, _TCHAR* argv[])
{
	char		szInBuf[1024]		= {0};
	char		szOutBuf[1024]		= {0};
	DWORD		dwOutSize			= sizeof(szOutBuf);
	DWORD		dwInSize			= sizeof(szInBuf);

	sprintf_s(szInBuf, "%d ’‚ «≤‚ ‘", GetCurrentProcessId());
	printf("PID: %d\n", GetCurrentProcessId());
	dwInSize = strlen(szInBuf) + 1;
	if(CallNamedPipe(_T("\\\\.\\pipe\\mytest")
		, szInBuf, dwInSize
		, szOutBuf, dwOutSize, &dwOutSize, NMPWAIT_USE_DEFAULT_WAIT))
	{
		printf("Call named pipe recv: %s\n", szOutBuf);
	}
	else
	{
		printf("Call named pipe failed: %d\n", GetLastError());
	}

	system("Pause");
	return 0;
}

