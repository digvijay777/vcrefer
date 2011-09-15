// TestQDBM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <windows.h>

#include "../qdbm/depot.h"

#pragma comment(lib, "../qdbm/qdbm_static.lib")

// ≤‚ ‘…Ë÷√
void testput()
{
	DWORD			dwTick			= 0;
	DWORD			dwMiTime		= -1;
	DWORD			dwAll			= 0;

	for(int i = 0; i < 10000; i++)
	{
		char		szPath[512]		= {0};
		char		szValue[1024]	= {0};
		DWORD		dwCount			= GetTickCount();
		DEPOT*		pDb				= dpopen("dptest.db", DP_OCREAT | DP_OREADER | DP_OWRITER, 0);

		sprintf_s(szPath, sizeof(szPath), "hello\\%x\\test", i);
		sprintf_s(szValue, sizeof(szPath), "value%d_1", i);
		dpput(pDb, szPath, -1, szValue, strlen(szValue), DP_DOVER);

		dpclose(pDb);
		dwCount = GetTickCount() - dwCount;
		if(dwCount > dwTick)
			dwTick = dwCount;
		if(dwCount < dwMiTime)
			dwMiTime = dwCount;
		dwAll += dwCount;
		Sleep(1);
	}

	printf("testset: MaxTime: %d, MinTime: %d, AllTime: %d\n", dwTick, dwMiTime, dwAll);
}
// ≤‚ ‘∂¡»°
void testread()
{
	DEPOT*		pDb				= dpopen("dptest.db", DP_OCREAT | DP_OREADER | DP_OWRITER, 0);
	int			nData[1024]		= {0};

	dpput(pDb, "SoftWare\\Txtws", -1, "Txtws", 6, DP_DCAT);
	dpput(pDb, "SoftWare\\GreenEHome", -1, "GreenEHome", 11, DP_DCAT);

	char * pd = dpget(pDb, "SoftWare", -1, 0, 1024, nData);
	if(NULL == pDb)
	{
		printf("error: %s\n", dperrmsg(dpecode));
		return ;
	}
	
	dpclose(pDb);
}
int _tmain(int argc, _TCHAR* argv[])
{
	testput();
	//testread();

	system("Pause");
	return 0;
}

