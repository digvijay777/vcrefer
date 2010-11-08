#include "StdAfx.h"
#include ".\forcefile.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib");

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
	return FALSE;
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

	if(!PathFileExists(lpFilePath))
		return TRUE;
	pExt = _tcsrchr(lpFilePath, _T('.'));
	if(NULL != pExt)
	{
		*pExt = '0';
		pExt++;
	}
	pRSign = _tcsrchr(lpFilePath, _T(')'));
	if(pRSign == (TCHAR*)(lpFilePath + _tcslen(lpFilePath) - 1))
	{
		pPoint = pLSign = _tcsrchr(lpFilePath, '(');
		for(; pPoint < pRSign; pPoint++)
		{
			if(*pPoint < '0' || *pPoint >'9')
				break;
		}
		if(pPoint == pRSign)
		{
			*pLSign = '\0';
			nIndex = _ttoi((TCHAR*)(pLSign + 1)) + 1;
		}
	}
	// 增长文件名
	for(; nIndex < 1024; nIndex++)
	{
		_stprintf(szNewPath, _T("%s(%d).%s"), lpFilePath, nIndex, pExt);
		if(!PathFileExists(szNewPath))
		{
			_tcscpy(lpFilePath, szNewPath);
			break;
		}
	}
	// 是否大于的1024个复本
	if(1024 >= nIndex)
	{
		*lpFilePath = 0;
		return FALSE;
	}

	return TRUE;
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
	FreeMemory();
	bRes = DeleteFile(m_strFilePath.c_str());
	if(FALSE != bRes)
		return FALSE;

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
	if(FALSE != bRes)
		return TRUE;
	// 从内存中移出
	FreeMemory();
	bRes = ::MoveFile(m_strFilePath.c_str(), szNewName);
	if(FALSE != bRes)
		return TRUE;

	if(FALSE == bMoveReboot)
		return FALSE;

	// 重启后移动

}