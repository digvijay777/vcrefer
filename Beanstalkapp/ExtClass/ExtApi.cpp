#include "stdafx.h"
#include "ExtApi.h"
#include <Tlhelp32.h>

/*
 *	����Ϣѭ���ĵȴ�
 */
DWORD ext::MessageLoop(HANDLE hHandle, DWORD dwMilliseconds)
{
	DWORD			dwRet		= WAIT_TIMEOUT;
	MSG				msg;
	DWORD			dwTick		= GetTickCount();
	DWORD			dwWaitObj	= 0;

	do 
	{
		// �ж��Ƿ�ʱ
		if((GetTickCount() - dwTick) >= dwMilliseconds)
			break;

		// ������Ϣ
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(WM_QUIT == msg.message)
				return 0;
			
			// MFC�ַ���Ϣ
			if(NULL != AfxGetApp())
			{
				if(AfxGetApp()->PumpMessage())
					continue;
			}
			
			// ��MFC�ַ���Ϣ
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}
		
		// �ȴ�
		dwWaitObj = ::MsgWaitForMultipleObjects(1, &hHandle, FALSE, 
			dwMilliseconds - (GetTickCount() - dwTick),	QS_ALLINPUT);

		// ����Ϣ�ĵ����ʹ�����Ϣ
		if((WAIT_OBJECT_0 + 1) == dwWaitObj)
			continue;
		
		// �ȴ��������˳�
		dwRet = dwWaitObj;
		break;
	} while (true);

	return dwRet;
}

/*
 *	��DLL�õ��ӿ�
 */
HRESULT ext::CreateInstanceFromDll(LPCTSTR lpDllName, REFCLSID rclsid, REFIID riid, LPVOID * ppv)
{
	HINSTANCE			hInst		= NULL;
	IClassFactory*		pFactory	= NULL;
	HRESULT				hr			= E_FAIL;

	(*ppv) = NULL;
	// ���ؿ��ļ�
	hInst = LoadLibrary( lpDllName );
	if (NULL == hInst)
	{
		return GetLastError();
	}

	// �õ�DllGetClassObject�������
	typedef HRESULT (__stdcall *GETCLASS_PROC)(REFCLSID, REFIID, LPVOID*);
	GETCLASS_PROC GetClassObject = (GETCLASS_PROC)GetProcAddress( hInst, "DllGetClassObject" ); 
	if(NULL == GetClassObject)
	{
		return GetLastError();
	}

	do 
	{
		// �õ��๤��
		hr = GetClassObject(rclsid, IID_IClassFactory, (void**)&pFactory);
		if(FAILED(hr) || (NULL == pFactory))
			break;

		hr = pFactory->CreateInstance( NULL, riid, ppv);

		pFactory->Release();
		pFactory = NULL;
	} while (FALSE);
	
	if(NULL == *ppv)
		FreeLibrary(hInst);
	
	return hr;
}

/*
 *	�ͷ�û��ʹ�õĿ�
 */
void ext::FreeUnusedLibraries(LPCTSTR lpDllName)
{
	HINSTANCE			hInst		= NULL;
	HANDLE				hToolHelp	= INVALID_HANDLE_VALUE;
	MODULEENTRY32		module;

	// ö�ٽ����ڵ�ģ��
	hToolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if(INVALID_HANDLE_VALUE == hToolHelp)
	{
		return;
	}
	Module32First(hToolHelp, &module);
	do 
	{
		if(_tcsicmp(module.szModule, lpDllName) == 0)
		{
			hInst = (HINSTANCE)module.hModule;
		}
	} while (Module32Next(hToolHelp, &module));
	CloseHandle(hToolHelp);
	// DLL�Ƿ�����ͷ�
	if(NULL == hInst)
		return;
	
	typedef HRESULT (_stdcall *CANUNLOAD_PROC)();
	CANUNLOAD_PROC DllCanUnloadNow = (CANUNLOAD_PROC)GetProcAddress(hInst, "DllCanUnloadNow");
	if(NULL == DllCanUnloadNow)
		return;

	if(S_OK == DllCanUnloadNow())
	{
		FreeLibrary(hInst);
	}
}