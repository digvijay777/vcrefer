#include "stdafx.h"
#include "DllInject.h"

// ����ʱע��DLL
BOOL dll::CreateProcessWithInject(LPCTSTR lpExe, LPTSTR lpCommand, LPCSTR lpDll)
{
	STARTUPINFO					si				= {0};
	PROCESS_INFORMATION			pi				= {0};
	BOOL						bRes			= FALSE;
	CHAR*						pFileName		= NULL;

	// �����߳�
	si.cb = sizeof(STARTUPINFO);
	// dll::EnableDebugPrivilege();
	bRes = CreateProcess(lpExe, lpCommand, NULL, NULL, FALSE
		, 0/*CREATE_SUSPENDED | DEBUG_ONLY_THIS_PROCESS | DEBUG_PROCESS*/
		, NULL, NULL, &si, &pi);
	if(FALSE != bRes)
	{
		Sleep(1);
		SuspendThread(pi.hThread);
		// ע��DLL
		do
		{
			if(NULL == lpDll)
				break;

			typedef struct tagInjectEsp{
				DWORD		dwCurAddr;
				DWORD		dwDllFileNamePtr;
			}INJECTESP;

			CONTEXT			context		= {0};
			DWORD			dwWrite		= 0;
			INJECTESP		esp;
			DWORD			dwOldProtect;

			context.ContextFlags = CONTEXT_FULL;	// û��ָ�����ʲôҲ�ò���
			GetThreadContext(pi.hThread, &context);
			// ��������
			pFileName = (CHAR *)VirtualAllocEx(pi.hProcess, 0, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if(FALSE == WriteProcessMemory(pi.hProcess, pFileName, lpDll
				, strlen(lpDll)+1, &dwWrite))
				break;
			esp.dwDllFileNamePtr = (DWORD)pFileName;
			esp.dwCurAddr = context.Eip;

			if(FALSE == WriteProcessMemory(pi.hProcess, (LPVOID)(context.Esp - sizeof(INJECTESP))
				, &esp, sizeof(INJECTESP), &dwWrite))
				break;
			context.Esp -= sizeof(INJECTESP);
			context.Eip = (DWORD)GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

			SetThreadContext(pi.hThread, &context);
		} while (FALSE);

		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	return TRUE;
}

BOOL dll::EnableDebugPrivilege()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES token_p;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken)) {
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&token_p.Privileges[0].Luid);//Ҫ����Խ��̵�Ȩ�� 
		token_p.PrivilegeCount=1;
		token_p.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		if(AdjustTokenPrivileges(hToken,FALSE,&token_p,sizeof(token_p),NULL,NULL))//����Ȩ��
			return TRUE;
	}
	return FALSE;
}