#include "StdAfx.h"
#include ".\forcefile.h"

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "shlwapi.lib")

CForceFile::CForceFile(LPCTSTR lpFilePath)
{
	m_bReboot = FALSE;		// ����Ҫ����
	m_strFilePath = lpFilePath;
}

CForceFile::~CForceFile(void)
{
}

// �Ƿ�Ҫ����
BOOL CForceFile::IsReboot()
{
	return m_bReboot;
}

// ���ڴ����Ƴ�
BOOL CForceFile::FreeMemory()
{
	std::vector<PROCESSENTRY32>		vctPE;
	std::vector<MODULEENTRY32>		vctME;
	BOOL							bRes		= FALSE;
	BOOL							bTerminate	= TRUE;

	// �õ����̿���
	if(FALSE == GetProcessSnapshot(vctPE))
		return FALSE;

	for(int i = 0; i < vctPE.size(); i++)
	{
		vctME.clear();
		// �õ�ģ�����
		if(FALSE == GetModuleSnapshot(vctME, vctPE[i].th32ProcessID))
			continue;

		for(int j = 0; j < vctME.size(); j++)
		{
			if(_tcsicmp(m_strFilePath.c_str(), vctME[j].szExePath) != 0)
				continue;

			// ���ؿ�
			if(FALSE != UnloadLibrary(vctME[j].hModule, vctPE[i].th32ProcessID, 
				vctME[j].ProccntUsage, vctME))
				continue;

			// ��������
			bTerminate &= TerminateProcess(vctPE[i].th32ProcessID);
			bRes = TRUE;
		}
	}
	return bRes & bTerminate;
}

// �õ������ļ���
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
			// ȥ��ǰ��Ŀո�
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
// ɾ���ļ�
BOOL CForceFile::Delete(BOOL bDelReboot /* = FALSE */)
{
	BOOL		bRes		= FALSE;

	if(!PathFileExists(m_strFilePath.c_str()))
		return TRUE;

	// ��ͨɾ��
	bRes = DeleteFile(m_strFilePath.c_str());
	if(FALSE != bRes)
		return TRUE;

	// ���ڴ����Ƴ�
	bRes = FreeMemory();

	// ��һ��ɾ��
	bRes = DeleteFile(m_strFilePath.c_str());
	if(FALSE != bRes)
		return TRUE;
	if(FALSE == bDelReboot)
		return FALSE;
    
	// ������ɾ��
	m_bReboot = MoveFileEx(m_strFilePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	return FALSE;
}

// �ƶ��ļ�
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
		// �õ������ļ���
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
	// ��ͨ�ƶ�
	bRes = ::MoveFile(m_strFilePath.c_str(), szNewName);
	if(FALSE == bRes)
	{
		FreeMemory();
		bRes = ::MoveFile(m_strFilePath.c_str(), szNewName);
	}

	// ����ʱ����
	if(FALSE == bRes && FALSE != bMoveReboot)
	{
		m_bReboot = MoveFileEx(m_strFilePath.c_str(), szNewName, MOVEFILE_DELAY_UNTIL_REBOOT);
		return FALSE;
	}

	return bRes;
}
// ����
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

// ����
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

	// ���ʱ��
	Time64ToString(_time64(NULL), szTime);
	_tcscat(szName, szTime);
	_tcscat(szNewPath, szName);

	bRet = MoveTo(szNewPath, bGrowName, bMoveReboot);
	// ��¼�����ļ�
	if(FALSE == bRet)
		return FALSE;

	_tcscat(szLog, "Virus.log");
	WritePrivateProfileString(_T("Move"), szName, m_strFilePath.c_str(), szLog);
	return bRet;
}

// ʱ��ת��
void CForceFile::Time64ToString(__time64_t t64, LPTSTR lpStr)
{
	_stprintf(lpStr, "%I64X", t64);
}
// �õ����̿���
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

// �õ�ģ�����
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

// ��������
BOOL CForceFile::TerminateProcess(DWORD dwProcessID)
{
	HANDLE				hProcess		= OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	BOOL				bRes			= FALSE;

	if(NULL == hProcess)
		return FALSE;

	bRes = ::TerminateProcess(hProcess, 0);
	return bRes;
}

// ж�ض�̬��
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

// �Ƿ�Ϊ����
BOOL CForceFile::IsNumber(LPCTSTR lpStr)
{
	for(int i = 0; lpStr[i] != 0 && lpStr [i] != _T(')'); i++)
	{
		if(lpStr[i] < _T('0') || lpStr[i] > _T('9'))
			return FALSE;
	}

	return TRUE;
}