#include "StdAfx.h"
#include ".\forcefile.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib");

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
	return FALSE;
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
	// �����ļ���
	for(; nIndex < 1024; nIndex++)
	{
		_stprintf(szNewPath, _T("%s(%d).%s"), lpFilePath, nIndex, pExt);
		if(!PathFileExists(szNewPath))
		{
			_tcscpy(lpFilePath, szNewPath);
			break;
		}
	}
	// �Ƿ���ڵ�1024������
	if(1024 >= nIndex)
	{
		*lpFilePath = 0;
		return FALSE;
	}

	return TRUE;
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
	FreeMemory();
	bRes = DeleteFile(m_strFilePath.c_str());
	if(FALSE != bRes)
		return FALSE;

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
	if(FALSE != bRes)
		return TRUE;
	// ���ڴ����Ƴ�
	FreeMemory();
	bRes = ::MoveFile(m_strFilePath.c_str(), szNewName);
	if(FALSE != bRes)
		return TRUE;

	if(FALSE == bMoveReboot)
		return FALSE;

	// �������ƶ�

}