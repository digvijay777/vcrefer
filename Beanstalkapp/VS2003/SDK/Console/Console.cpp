// Console.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE		hStdOut			= GetStdHandle(STD_OUTPUT_HANDLE);
	TCHAR		szOut[1024]		= {0};
	COORD		pos				= {10, 20};
	DWORD		dwWrite			= 0;
	WORD		wColor[1024]	= {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	CONSOLE_CURSOR_INFO 	cci = {0};
	CONSOLE_SCREEN_BUFFER_INFO	csbi = {0};
	const TCHAR* strPrompt		= "请输入一个字符串:";
	const TCHAR* strPrompt2		= "你的输入是:\t%s\t\t\n";

	int n = sizeof(strPrompt);
	int m = sizeof(szOut);
// 	int o = _countof(strPrompt);
// 	int p = _countof(szOut);
	while(1)
	{
		SetConsoleTitle(_T("Console 测试"));
		SetConsoleTextAttribute(hStdOut, 4);
		//WriteConsoleOutputAttribute(hStdOut, wColor, _tcslen(szOut), csbi.dwCursorPosition, &dwWrite);
		//WriteConsoleOutputCharacter(hStdOut, szOut, _tcslen(szOut), csbi.dwCursorPosition, &dwWrite);
		//csbi.dwCursorPosition.X += dwWrite;
		WriteConsole(hStdOut, strPrompt, _tcslen(strPrompt), &dwWrite, NULL);
	//	CloseHandle(hStdOut);
		SetConsoleTextAttribute(hStdOut, 5);
		scanf("%s", szOut);
		SetConsoleTextAttribute(hStdOut, 6);

		GetConsoleScreenBufferInfo(hStdOut, &csbi);
		pos = csbi.dwCursorPosition;
		pos.Y -= 1;
		SetConsoleCursorPosition(hStdOut, pos);
		//GetConsoleCursorInfo(hStdOut, &cci);
		//SetConsoleCursorInfo(hStdOut, &cci);

		printf(strPrompt2, szOut);
		if(_tcsicmp(szOut, _T("exit")) == 0)
			break;
	}
	system("pause");
	return 0;
}

