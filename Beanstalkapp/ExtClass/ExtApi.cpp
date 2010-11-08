#include "stdafx.h"
#include "ExtApi.h"
#include <Tlhelp32.h>

/*
 *	带消息循环的等待
 */
DWORD ext::MessageLoop(HANDLE hHandle, DWORD dwMilliseconds)
{
	DWORD			dwRet		= WAIT_TIMEOUT;
	MSG				msg;
	DWORD			dwTick		= GetTickCount();
	DWORD			dwWaitObj	= 0;

	do 
	{
		// 判断是否超时
		if((GetTickCount() - dwTick) >= dwMilliseconds)
			break;

		// 处理消息
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(WM_QUIT == msg.message)
				return 0;
			
			// MFC分发消息
			if(NULL != AfxGetApp())
			{
				if(AfxGetApp()->PumpMessage())
					continue;
			}
			
			// 非MFC分发消息
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}
		
		// 等待
		dwWaitObj = ::MsgWaitForMultipleObjects(1, &hHandle, FALSE, 
			dwMilliseconds - (GetTickCount() - dwTick),	QS_ALLINPUT);

		// 是消息的到来就处理消息
		if((WAIT_OBJECT_0 + 1) == dwWaitObj)
			continue;
		
		// 等待结束，退出
		dwRet = dwWaitObj;
		break;
	} while (true);

	return dwRet;
}

/*
 *	从DLL得到接口
 */
HRESULT ext::CreateInstanceFromDll(LPCTSTR lpDllName, REFCLSID rclsid, REFIID riid, LPVOID * ppv)
{
	HINSTANCE			hInst		= NULL;
	IClassFactory*		pFactory	= NULL;
	HRESULT				hr			= E_FAIL;

	(*ppv) = NULL;
	// 加载库文件
	hInst = LoadLibrary( lpDllName );
	if (NULL == hInst)
	{
		return GetLastError();
	}

	// 得到DllGetClassObject函数入口
	typedef HRESULT (__stdcall *GETCLASS_PROC)(REFCLSID, REFIID, LPVOID*);
	GETCLASS_PROC GetClassObject = (GETCLASS_PROC)GetProcAddress( hInst, "DllGetClassObject" ); 
	if(NULL == GetClassObject)
	{
		return GetLastError();
	}

	do 
	{
		// 得到类工厂
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
 *	释放没有使用的库
 */
void ext::FreeUnusedLibraries(LPCTSTR lpDllName)
{
	HINSTANCE			hInst		= NULL;
	HANDLE				hToolHelp	= INVALID_HANDLE_VALUE;
	MODULEENTRY32		module;

	// 枚举进程内的模块
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
	// DLL是否可以释放
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