// testiobuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <WinError.h>
#include <sys/stat.h>
#include <windows.h>
#include <WinIoCtl.h>
#include "../XRegmonCtrl.h"


void WriteParamter( void );

int _tmain(int argc, const char* argv[])
{
	HANDLE		hFile	= CreateFile("\\\\.\\XRegmon" /*"\\\\.\\TestIODrect"*/ /*"\\\\.\\TestIOBuffer"*/
		, GENERIC_READ|GENERIC_WRITE
		, 0
		, NULL
		, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL
		, NULL);
	DWORD			dwOut;
	FindBadOptItem	item		= {0};

	if(INVALID_HANDLE_VALUE == hFile)
	{
		printf("���ܴ��ļ�: \"\\\\.\\XRegmon\" %d\n"
			, GetLastError());
		return ERROR_INVALID_FIELD;
	}
	DeviceIoControl(hFile, IOCTRL_REGMON_START, NULL, 0
		, NULL, 0, (DWORD *)&dwOut, NULL);
	printf("��ʼ��� %d\n", GetLastError());
	if(0 != GetLastError())
	{
		CloseHandle(hFile);
		return 0;
	}
	while(ReadFile(hFile, &item, sizeof(item), &dwOut, NULL))
	{
		LONG		nAllowd		= 0;

		printf("���ֳ���[PID:%d]: \"%S\" ����ע��� \"%S\\%S\" ��ֵΪ \"%S\" �Ƿ��������(Y/N):"
			, item.nPID
			, item.szExePath
			, item.szRegPath
			, item.szName
			, item.szValue);

		char		cy			= getchar();
		if('y' == cy || 'Y' == cy)
		{
			nAllowd = 1;
			printf("\n�������˲���\n");
		}
		else
		{
			printf("\n��ܾ��˲���\n");
		}
		WriteFile(hFile, &nAllowd, 4, &dwOut, NULL);
	}
	DeviceIoControl(hFile, IOCTRL_REGMON_STOP, NULL, 0
		, NULL, 0, (DWORD *)&dwOut, NULL);
	printf("ֹͣ��� %d\n", GetLastError());
	CloseHandle(hFile);

	return 0;
}

void WriteParamter( void )
{
	printf("\n������ʽ: -[w/r] -p[offsert number] ��������\n");
}

