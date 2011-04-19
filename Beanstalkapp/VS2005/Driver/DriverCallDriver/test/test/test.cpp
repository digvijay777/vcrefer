// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE		hFile;
	
	if(argc < 2)
	{
		printf("�������豸��.\n");
		return 0;
	}

	hFile = CreateFile(argv[1], GENERIC_READ
		, 0
		, NULL
		, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL
		, NULL);
	if(INVALID_HANDLE_VALUE == hFile || NULL == hFile)
	{
		printf("���豸'%s'ʧ��: %d\n", argv[1], GetLastError());
		return 0;
	}
	CHAR		szRead[50]	= {0};
	DWORD		dwSize		= sizeof(szRead);

	printf("[1]��ʼ��ȡ�豸: %d\n", GetTickCount());
	ReadFile(hFile, szRead, dwSize, &dwSize, NULL);
	printf("[1]��ȡ�豸���: %d %s\n", GetTickCount(), szRead);
	//Sleep(1000);
	dwSize = sizeof(szRead);
	printf("[2]��ʼ��ȡ�豸: %d\n", GetTickCount());
	ReadFile(hFile, szRead, dwSize, &dwSize, NULL);
	printf("[2]��ȡ�豸���: %d %s\n", GetTickCount(), szRead);
	dwSize = sizeof(szRead);
	//Sleep(1000);
	printf("[3]��ʼ��ȡ�豸: %d\n", GetTickCount());
	ReadFile(hFile, szRead, dwSize, &dwSize, NULL);
	printf("[3]��ȡ�豸���: %d %s\n", GetTickCount(), szRead);
	//Sleep(1000);
	CloseHandle(hFile);

	return 0;
}

