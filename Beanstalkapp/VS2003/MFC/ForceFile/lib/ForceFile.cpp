#include "StdAfx.h"
#include ".\forcefile.h"

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "shlwapi.lib")

CForceFile::CForceFile(LPCTSTR lpFilePath)
{
	m_bReboot = FALSE;		// 不需要重启
	m_strFilePath = lpFilePath;
}

CForceFile::~CForceFile(void)
{
}

// 是否要重启
BOOL CForceFile::IsReboot()
{
	return m_bReboot;
}

// 从内存中移出
BOOL CForceFile::FreeMemory()
{
	std::vector<PROCESSENTRY32>		vctPE;
	std::vector<MODULEENTRY32>		vctME;
	BOOL							bRes		= FALSE;
	BOOL							bTerminate	= TRUE;

	// 得到进程快照
	if(FALSE == GetProcessSnapshot(vctPE))
		return FALSE;

	for(int i = 0; i < vctPE.size(); i++)
	{
		vctME.clear();
		// 得到模块快照
		if(FALSE == GetModuleSnapshot(vctME, vctPE[i].th32ProcessID))
			continue;

		for(int j = 0; j < vctME.size(); j++)
		{
			if(_tcsicmp(m_strFilePath.c_str(), vctME[j].szExePath) != 0)
				continue;

			// 御载库
			if(FALSE != UnloadLibrary(vctME[j].hModule, vctPE[i].th32ProcessID, 
				vctME[j].ProccntUsage, vctME))
				continue;

			// 结束进程
			bTerminate &= TerminateProcess(vctPE[i].th32ProcessID);
			bRes = TRUE;
		}
	}
	return bRes & bTerminate;
}

// 得到增长文件名
BOOL CForceFile::GetGrowFileName(LPTSTR lpFilePath)
{
	BOOL		bRes		= 0;
	int			nIndex		= 1;
	TCHAR*		pPoint		= NULL;
	TCHAR*		pRSign		= NULL;
	TCHAR*		pLSign		= NULL;
	TCHAR*		pExt		= NULL;
	TCHAR		szNewPath[MAX_PATH*2]		= {0};
	TCHAR		szPrePath[MAX_PATH*2]		= {0};

	pExt = _tcsrchr(lpFilePath, _T('.'));
	if(NULL != pExt)
	{
		*pExt = 0;
		pExt++;
	}
	pRSign = pExt-2;
	pLSign = _tcsrchr(lpFilePath, _T('('));
	if(NULL != pLSign && _T(')') == *pRSign)
	{
		if(IsNumber((TCHAR*)(pLSign+1)))
		{
			*pRSign = 0;
			nIndex = _ttoi((TCHAR*)(pLSign+1));
			*pLSign = 0;
			// 去除前面的空格
			if(*(pLSign-1) == _T('\x20'))
				*(pLSign-1) = 0;
		}
	}
	_tcscpy(szPrePath, lpFilePath);
	_tcscat(szPrePath, " (%d).");
	_tcscat(szPrePath, pExt);
	for(_tcscpy(szNewPath, lpFilePath)
		; nIndex < 100
		; nIndex++)
	{
		_stprintf(szNewPath, szPrePath, nIndex);
		if(PathFileExists(szNewPath))
			continue;
		_tcscpy(lpFilePath, szNewPath);
		return TRUE;
	}

	return FALSE;
}
// 删除文件
BOOL CForceFile::Delete(BOOL bDelReboot /* = FALSE */)
{
	BOOL		bRes		= FALSE;

	if(!PathFileExists(m_strFilePath.c_str()))
		return TRUE;

	// 普通删除
	bRes = DeleteFile(m_strFilePath.c_str());
	if(FALSE != bRes)
		return TRUE;

	// 从内存中移出
	bRes = FreeMemory();

	// 再一次删除
	bRes = DeleteFile(m_strFilePath.c_str());
	if(FALSE != bRes)
		return TRUE;
	if(FALSE == bDelReboot)
		return FALSE;
    
	// 重启后删除
	m_bReboot = MoveFileEx(m_strFilePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	return FALSE;
}

// 移动文件
BOOL CForceFile::MoveTo(LPCTSTR lpNewPath, BOOL bGrowName /* = FALSE */, BOOL bMoveReboot /* = FALSE */)
{
	BOOL		bRes					= FALSE;
	TCHAR		szNewName[MAX_PATH*2]	= {0};

	if(!PathFileExists(m_strFilePath.c_str()))
		return FALSE;
	_tcscpy(szNewName, lpNewPath);
	if(PathFileExists(szNewName))
	{
		if(FALSE == bGrowName)
			return FALSE;
		// 得到增长文件名
		bRes = GetGrowFileName(szNewName);
		if(FALSE == bRes && FALSE != bMoveReboot)
		{
			m_bReboot = MoveFileEx(m_strFilePath.c_str(), lpNewPath, MOVEFILE_DELAY_UNTIL_REBOOT);
			return FALSE;
		}
		else if(FALSE == bRes)
		{
			return FALSE;
		}
	}
	// 普通移动
	bRes = ::MoveFile(m_strFilePath.c_str(), szNewName);
	if(FALSE == bRes)
	{
		FreeMemory();
		bRes = ::MoveFile(m_strFilePath.c_str(), szNewName);
	}

	// 重启时操作
	if(FALSE == bRes && FALSE != bMoveReboot)
	{
		m_bReboot = MoveFileEx(m_strFilePath.c_str(), szNewName, MOVEFILE_DELAY_UNTIL_REBOOT);
		return FALSE;
	}

	return bRes;
}
// 改名
BOOL CForceFile::RepRename(LPCTSTR lpExePre, BOOL bGrowName /* = FALSE */, BOOL bMoveReboot /* = FALSE */)
{
	TCHAR			szNewPath[MAX_PATH*2]		= {0};
	TCHAR			szExe[50]					= {0};
	TCHAR*			pSplit						= NULL;

	_tcscpy(szNewPath, m_strFilePath.c_str());
	pSplit = _tcsrchr(szNewPath, _T('.'));
	if(NULL != pSplit)
	{
		*pSplit = 0;
		pSplit++;
		_tcscpy(szExe, pSplit);
	}
	_tcscat(szNewPath, ".");
	_tcscat(szNewPath, lpExePre);
	_tcscat(szNewPath, szExe);

	return MoveTo(szNewPath, bGrowName, bMoveReboot);
}

// 隔离
BOOL CForceFile::RepMove(LPCTSTR lpRepPath, BOOL bGrowName /* = FALSE */, BOOL bMoveReboot /* = FALSE */)
{
	BOOL		bRet					= FALSE;
	TCHAR		szNewPath[MAX_PATH*2]	= {0};
	TCHAR		szName[MAX_PATH]		= {0};
	TCHAR*		pSplit					= NULL;
	TCHAR		szLog[MAX_PATH*2]		= {0};
	TCHAR		szTime[10]				= {0};

	_tcscpy(szNewPath, lpRepPath);
	if(lpRepPath[_tcslen(lpRepPath)-1] != _T('\\'))
		_tcscat(szNewPath, _T("\\"));
	_tcscpy(szLog, szNewPath);
	pSplit = _tcsrchr(m_strFilePath.c_str(), _T('\\'));
	if(NULL != pSplit)
		_tcscpy(szName, (TCHAR *)(pSplit+1));
	_tcscat(szName, _T("."));

	// 添加时间
	Time64ToString(_time64(NULL), szTime);
	_tcscat(szName, szTime);
	_tcscat(szNewPath, szName);

	bRet = MoveTo(szNewPath, bGrowName, bMoveReboot);
	// 记录隔离文件
	if(FALSE == bRet)
		return FALSE;

	_tcscat(szLog, "Virus.log");
	WritePrivateProfileString(_T("Move"), szName, m_strFilePath.c_str(), szLog);
	return bRet;
}

// 时间转换
void CForceFile::Time64ToString(__time64_t t64, LPTSTR lpStr)
{
	_stprintf(lpStr, "%I64X", t64);
}
// 得到进程快照
BOOL CForceFile::GetProcessSnapshot(std::vector<PROCESSENTRY32>& vctPE)
{
	PROCESSENTRY32			pe			= {0};
	HANDLE					hSnapshot	= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL					bRes;

	if(INVALID_HANDLE_VALUE == hSnapshot)
		return FALSE;

	pe.dwSize = sizeof(pe);
	for(bRes = Process32First(hSnapshot, &pe)
		; FALSE != bRes
		; bRes = Process32Next(hSnapshot, &pe))
	{
		vctPE.push_back(pe);
	}
	CloseHandle(hSnapshot);
	
	return TRUE;
}

// 得到模块快照
BOOL CForceFile::GetModuleSnapshot(std::vector<MODULEENTRY32>& vctME, DWORD dwProcessID /* = 0 */)
{
	MODULEENTRY32			me			= {0};
	HANDLE					hSnapshot	= CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	BOOL					bRes;

	if(INVALID_HANDLE_VALUE == hSnapshot)
		return FALSE;

	me.dwSize = sizeof(me);
	for(bRes = Module32First(hSnapshot, &me)
		; FALSE != bRes
		; bRes = Module32Next(hSnapshot, &me))
	{
		vctME.push_back(me);
	}
	CloseHandle(hSnapshot);

	return TRUE;
}

// 结束进程
BOOL CForceFile::TerminateProcess(DWORD dwProcessID)
{
	HANDLE				hProcess		= OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	BOOL				bRes			= FALSE;

	if(NULL == hProcess)
		return FALSE;

	bRes = ::TerminateProcess(hProcess, 0);
	return bRes;
}

// 卸载动态库
BOOL CForceFile::UnloadLibrary(HMODULE hModule, DWORD dwProcessID, int nProccntUsage, std::vector<MODULEENTRY32>& vctME)
{
	LPTHREAD_START_ROUTINE			fnFreeLibrary		= NULL;

	if((WORD)-1 == nProccntUsage)
		return FALSE;

	for(int i = 0; i < vctME.size(); i++)
	{
		fnFreeLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(vctME[i].hModule, "FreeLibrary");
		if(NULL != fnFreeLibrary)
			break;
	}
	
	if(NULL == fnFreeLibrary)
		return FALSE;

	HANDLE							hProcess			= OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	HANDLE							hRemote				= NULL;
	DWORD							dwCode				= 0;
	int								nSuccess			= 0;

	if(NULL == hProcess)
		return FALSE;
	
	for(int i = 0; i < nProccntUsage; i++)
	{
		hRemote = CreateRemoteThread(hProcess, NULL, NULL, fnFreeLibrary, (void *)hModule, 0, NULL);
		if(NULL == hRemote)
			return FALSE;

		WaitForSingleObject(hRemote, INFINITE);
		GetExitCodeThread(hRemote, &dwCode);
		CloseHandle(hRemote);
		if(FALSE != dwCode)
			nSuccess++;
	}

	CloseHandle(hProcess);
	return (nSuccess == nProccntUsage);
}

// 是否为数字
BOOL CForceFile::IsNumber(LPCTSTR lpStr)
{
	for(int i = 0; lpStr[i] != 0 && lpStr [i] != _T(')'); i++)
	{
		if(lpStr[i] < _T('0') || lpStr[i] > _T('9'))
			return FALSE;
	}

	return TRUE;
}