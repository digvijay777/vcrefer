// ODBC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <sql.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SQLHENV			hEnv;
	SQLRETURN		retCode;
	SQLHDBC			hDbc;
	// step 1: 连接数据源
	retCode = ::SQLAllocEnv(&hEnv);
	if(SQL_SUCCESS != retCode)
		return -1;
	retCode = ::SQLAllocConnect(hEnv, &hDbc);
	if(SQL_SUCCESS != retCode)
		goto end_1;
	

end_2:
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
end_1:
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	return 0;
}

