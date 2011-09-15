// FirstSQLite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

extern "C"
{
	#include "../SQLite/sqlite3.h"
};

static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName)
{
	int i;
	for ( i=0; i < argc; i++ )
	{
		printf( "%s = %s\n", szColName[i], argv[i] == 0 ? "NUL" : argv[i] );
	}

	return 0;
}

void testnormal()
{
	sqlite3*		db			= NULL;
	int				nRet		= 0;
	char*			pErrMsg		= NULL;

	nRet = sqlite3_open("first.db", &db);
	if(SQLITE_OK != nRet)
	{
		fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
	}

	fprintf(stdout, "数据库连接成功!\n");

	// 方法一
	fprintf(stdout, "\n\n********************\n操作方式一:\n");
	nRet = sqlite3_exec(db, "select * from class limit 10", _sql_callback, 0, &pErrMsg);
	if(SQLITE_OK != nRet)
	{
		fprintf(stderr, "SQL error: %s\n", pErrMsg);
		sqlite3_free(pErrMsg);
	}
	// 方法二 
	fprintf(stdout, "\n\n********************\n操作方式二:\n");
	char**		dbTable;
	int			nRows			= 0;
	int			nColumns		= 0;

	nRet = sqlite3_get_table(db, "select * from class limit 1,1", &dbTable, &nRows, &nColumns, &pErrMsg);
	if(SQLITE_OK == nRet)
	{
		int		nIndex		= 0;

		for(int i = 0; i < nColumns; i++)
		{
			fprintf(stdout, "%s\t", dbTable[i]);
		}
		fprintf(stdout, "\n");
		for(int i = 0; i < nRows; i++)
		{
			for(int j = 0; j < nColumns; j++)
			{
				fprintf(stdout, "%s\t", dbTable[nColumns + i * nColumns + j]);
			}
			fprintf(stdout, "\n");
		}
		sqlite3_free_table(dbTable);
	}
	else
	{
		fprintf(stderr, "SQL error: %s\n", pErrMsg);
		sqlite3_free(pErrMsg);
	}

	sqlite3_close(db);
}

void testinsert()
{
	DWORD			dwMaxTime	= 0;
	DWORD			dwMinTime	= -1;
	DWORD			dwAll		= 0;
	int				nRet		= 0;
	sqlite3*		db			= NULL;

	nRet = sqlite3_open("test.db", &db);
	for(int i = 0; i < 10000; i++, Sleep(1))
	{
		CHAR		szValue[1024];
		DWORD		dwTick;
		char*		pErr		= NULL;

		dwTick = GetTickCount();
		if(SQLITE_OK != nRet)
		{
			fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
		}

		sprintf(szValue, "insert into class values(%d, 'hi', %d)"
			, i, i % 5 + 20);
		nRet = sqlite3_exec(db, szValue, NULL, 0, &pErr);
		if(SQLITE_OK != nRet)
		{
			fprintf(stderr, "SQL error: %s\n", pErr);
		}

		dwTick = GetTickCount() - dwTick;
		if(dwTick > dwMaxTime)
			dwMaxTime = dwTick;
		if(dwTick < dwMinTime)
			dwMinTime = dwTick;
		dwAll += dwTick;
	}
	sqlite3_close(db);
	
	printf("MaxTime: %d, MinTime: %d, AllTime: %d\n", dwMaxTime, dwMinTime, dwAll);
}

int _tmain(int argc, _TCHAR* argv[])
{
//	testnormal();

	testinsert();
	// 数据库加密
// 	char*		pKeyWork		= "xxj123";
// 	sqlite3_key(db, pKeyWork, strlen(pKeyWork));
	system("pause");
	return 0;
}

