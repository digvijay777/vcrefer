// printf.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdarg.h>
#include <time.h>

void MyPrinf(const char* format, ...)
{
	char		buffer[1024]	= {0};
	va_list		args;
	char		tbuffer [9];

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	printf("%s:\t%s \r\n", _strtime(tbuffer), buffer);
}

int _tmain(int argc, _TCHAR* argv[])
{
	MyPrinf("hello.\t%s\t%d\t%f", "yuanjie", 255, 5.3);
	system("pause");
// 	CFile::Open();
// 	CFile::GetLength();
// 	CFile::Seek();
// 	CStdioFile::ReadString();
// 	CMemFile::Attach();
// 	IMPLEMENT_SERIAL();VERSIONABLE_SCHEMA
	return 0;
}

