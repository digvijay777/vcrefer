#pragma once
#include <string>
#include <Tlhelp32.h>
#include <shlwapi.h>
#include <vector>

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
	BOOL	IsReboot();
	BOOL	IsNumber(LPCTSTR lpStr);

	BOOL	RepRename(LPCTSTR lpExePre, BOOL bGrowName = FALSE, BOOL bMoveReboot = FALSE);
	BOOL	RepMove(LPCTSTR lpRepPath, BOOL bGrowName = FALSE, BOOL bMoveReboot = FALSE);
private:
	BOOL	GetProcessSnapshot(std::vector<PROCESSENTRY32>& vctPE);
	BOOL	GetModuleSnapshot(std::vector<MODULEENTRY32>& vctME, DWORD dwProcessID = 0);
	BOOL	TerminateProcess(DWORD dwProcessID);
	BOOL	UnloadLibrary(HMODULE hModule, DWORD dwProcessID, int nProccntUsage, std::vector<MODULEENTRY32>& vctME);
	BOOL	GetGrowFileName(LPTSTR lpFilePath);
	void	Time64ToString(__time64_t t64, LPTSTR lpStr);
private:
	BOOL		m_bReboot;
	String		m_strFilePath;
};
