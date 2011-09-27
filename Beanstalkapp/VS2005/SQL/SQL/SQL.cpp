// SQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "TSQLString.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TSQLString<100>		sql;

	sql.format("insert into '%2s' '%d'", "'fds'fd'", 56);
	return 0;
}

