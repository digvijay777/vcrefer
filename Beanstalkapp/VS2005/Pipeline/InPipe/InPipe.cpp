// InPipe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>


int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO		si		= {0};

	GetStartupInfo(&si);

	if(NULL != si.hStdInput && INVALID_HANDLE_VALUE != si.hStdInput)
	{
		CHAR		szData[1024]		= {0};
		DWORD		dwRead				= 0;

		printf("开始读取数: \n");
		ReadFile(si.hStdInput, szData, sizeof(szData), &dwRead, NULL);
		printf(szData);
		//MessageBox(NULL, szData, NULL, MB_OK);
	}
	else
	{
		printf("没有要读取的数据: %d\n", si.hStdInput);
	}

	return 0;
}

