// CloudCenter.cpp : Implementation of CCloudCenter

#include "stdafx.h"
#include "CloudCenter.h"
#include ".\cloudcenter.h"

// 检查用户
DWORD CCloudCenter::CheckLoginThread(LPVOID lpParam)
{
	CCloudCenter*	pThis	= (CCloudCenter *)lpParam;

	return pThis->CheckLogin();
}
DWORD CCloudCenter::CheckLogin()
{
	Sleep(1000);
	return 1;	// 登录成功
}

// CCloudCenter
STDMETHODIMP CCloudCenter::Login(BSTR strCookie, VARIANT_BOOL *pbSuccess)
{
	HANDLE		hThread;
	DWORD		dwRes;
	DWORD		dwState		= -1;

	wcsncpy(m_szData, strCookie, sizeof(m_szData) / sizeof(wchar_t));
	hThread = CreateThread(NULL, 0, CheckLoginThread, this, 0, NULL);
	
	// 得到返回值
	while(true)
	{
		MSG msg; 

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			switch(msg.message)
			{
			case WM_QUIT:
			case WM_CLOSE:
			case WM_DESTROY:
				continue;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(WAIT_OBJECT_0 == WaitForSingleObject(hThread, 0))
				break;
		}
		dwRes = MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT|QS_ALLPOSTMESSAGE);
		if(WAIT_OBJECT_0 == dwRes)
		{
			GetExitCodeThread(hThread, &dwState);
			break;
		}
	}
	CloseHandle(hThread);
	// 发送事件
	if(0 == dwState)
		*pbSuccess = VARIANT_FALSE;
	else
		*pbSuccess = VARIANT_TRUE;

	return S_OK;
}


