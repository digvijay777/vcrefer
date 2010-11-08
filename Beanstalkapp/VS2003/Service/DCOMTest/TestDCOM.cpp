// TestDCOM.cpp : Implementation of CTestDCOM

#include "stdafx.h"
#include "TestDCOM.h"
#include ".\testdcom.h"

#include <WtsApi32.h>

#pragma comment(lib, "Wtsapi32.lib")
// CTestDCOM


STDMETHODIMP CTestDCOM::MessageBox(BSTR pMsg)
{
/*
	DWORD*		pSessionID		= NULL;
	DWORD		nSize			= NULL;
	BOOL		bRet			= FALSE;

	bRet = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE
		, WTS_CURRENT_SESSION
		, WTSSessionId
		, pSessionID
		, &nSize);
	if(FALSE == bRet)
		return S_OK;

	do 
	{
		HANDLE		hToken			= NULL;

		bRet = WTSQueryUserToken((ULONG)*pSessionID, &hToken);
		if(FALSE == bRet)
			break;

	} while (FALSE);

	WTSFreeMemory(pSessionID);*/

	DWORD		dwSessionID		= 0;
	BOOL		bRet			= FALSE;
	DWORD		dwRet			= 0;
	WCHAR*		lpTitle			= L"DCOMTest";

// 	dwSessionID = WTSGetActiveConsoleSessionId();
// 	if(0 == dwSessionID)
// 		return S_OK;

	WTSSendMessageW(WTS_CURRENT_SERVER_HANDLE
		, WTS_CURRENT_SESSION
		, lpTitle
		, wcslen(lpTitle) * sizeof(WCHAR)
		, pMsg
		, wcslen(pMsg) * sizeof(WCHAR)
		, MB_OK | MB_ICONINFORMATION
		, 0
		, &dwRet
		, FALSE);

	return S_OK;
}
