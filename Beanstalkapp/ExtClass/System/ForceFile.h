#pragma once
#include <string>

#if defined(_UNICODE) || defined(UNICODE)
	#define String		std::wstring
#else
	#define String		std::string
#endif

class CForceFile
{
public:
	CForceFile(LPCTSTR lpFilePath);
	virtual ~CForceFile(void);
public:
	BOOL	FreeMemory();
	BOOL	Delete(BOOL bDelReboot = FALSE);
	BOOL	MoveTo(LPCTSTR lpNewPath, BOOL bGrowName = FALSE, BOOL bMoveReboot = FALSE);
	BOOL	Rename(LPCTSTR lpNewPath, BOOL bGrowName = FALSE, BOOL bMoveReboot = FALSE);
	BOOL	IsReboot();
private:
	BOOL	GetGrowFileName(LPTSTR lpFilePath);		
private:
	BOOL		m_bReboot;
	String		m_strFilePath;
};
