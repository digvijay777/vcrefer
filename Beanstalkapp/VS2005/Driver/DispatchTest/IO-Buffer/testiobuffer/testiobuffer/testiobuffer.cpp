// testiobuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <WinError.h>
#include <sys/stat.h>
#include <windows.h>
#include <WinIoCtl.h>

#define IOCTRL_TEST1			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x800 \
	, METHOD_BUFFERED \
	, FILE_ANY_ACCESS)

#define IOCTRL_TEST2			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x801 \
	, METHOD_IN_DIRECT \
	, FILE_ANY_ACCESS)


void WriteParamter( void );

int _tmain(int argc, const char* argv[])
{
	atexit(WriteParamter);

	if(argc < 3)
	{
		return ERROR_INVALID_PARAMETER;
	}

	int		nSize		= 0;

	if(_strnicmp("-p", argv[2], 2) == 0)
	{
		nSize = atoi(&argv[2][2]);
	}

	if(_stricmp("-w", argv[1]) == 0)
	{
		HANDLE		hFile	= CreateFile("\\\\.\\TestIOControl" /*"\\\\.\\TestIODrect"*/ /*"\\\\.\\TestIOBuffer"*/
			, GENERIC_READ|GENERIC_WRITE
			, 0
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL
			, NULL);

		if(INVALID_HANDLE_VALUE == hFile)
		{
			printf("不能打开文件: \"\\\\.\\TestIOBuffer\" %d\n"
				, GetLastError());
			return ERROR_INVALID_FIELD;
		}
		SetFilePointer(hFile, nSize, NULL, FILE_BEGIN);

		// ioctrl
		CHAR		szBuff[2048]		= {0};

		for(int i = 3; i < argc; i++)
		{
			strcat(szBuff, argv[i]);
			strcat(szBuff, " ");
		}

		LONG		nWrite		= 0;
		LONG		nOut		= sizeof(nWrite);

		DeviceIoControl(hFile, IOCTRL_TEST1, szBuff, strlen(szBuff)
			, &nWrite, sizeof(nWrite), (DWORD *)&nOut, NULL);
		printf("[DeviceIoControl] write %d byte.\r\n", nWrite);
		// write
// 		for(int i = 3; i < argc; i++)
// 		{
// 			int		nws		= strlen(argv[i])+1;
// 			DWORD	nraw	= 0;
// 
// 			WriteFile(hFile, argv[i], nws, &nraw, NULL);
// 			//SetFilePointer(hFile, nraw, NULL, FILE_CURRENT);
// 			if(nws != nraw)
// 			{
// 				printf("文件已到结束");
// 				break;
// 			}
// 		}
		CloseHandle(hFile);
	}
	else if(_stricmp("-r", argv[1]) == 0)
	{
		HANDLE		hFile	= CreateFile("\\\\.\\TestIOControl" /*"\\\\.\\TestIODrect"*/ /*"\\\\.\\TestIOBuffer"*/
			, GENERIC_READ|GENERIC_WRITE
			, 0
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL
			, NULL);
		int			nFileSize	= 0;
		char*		pData	= NULL;
		int			nrs		= 0;

		if(INVALID_HANDLE_VALUE == hFile)
		{
			printf("不能打开文件: \"\\\\.\\TestIOBuffer\" %d\n"
				, GetLastError());
			return ERROR_INVALID_FIELD;
		}
		nFileSize = GetFileSize(hFile, NULL);
		if(nFileSize <= nSize)
		{
			printf("超出读取文件大小(%d)\n", nFileSize);
			CloseHandle(hFile);
			return -1;
		}
		SetFilePointer(hFile, nSize, NULL, FILE_BEGIN);
		nrs = nFileSize - nSize;
		pData = (char *)malloc(nrs);
		memset(pData, 0, nrs);
		ReadFile(hFile, pData, nrs, (LPDWORD)&nrs, NULL);
		CloseHandle(hFile);

		char*	pp	= pData;

		for(int i = 0; pp - pData < nrs; i++, pp += strlen(pp)+1)
		{
			printf("read data[%02d]: %s\n", i, pp);
		}
		free(pData);
	}
	else
	{
		printf("未知操作方式[%s].\n", argv[1]);
		return ERROR_INVALID_PARAMETER;
	}
	return 0;
}

void WriteParamter( void )
{
	printf("\n参数格式: -[w/r] -p[offsert number] 附加数据\n");
}

