// WindowStation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

BOOL CALLBACK EnumWindowStationProc(LPSTR  lpszWindowStation, LPARAM lParam)
{
	printf("Find station: %s\n", lpszWindowStation);

	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	EnumWindowStations(EnumWindowStationProc, NULL);
	system("Pause");
	return 0;
}

