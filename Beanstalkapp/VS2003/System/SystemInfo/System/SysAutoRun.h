#pragma once
#include <vector>
#include <string>

class CSysAutoRun
{
public:
	CSysAutoRun();
	virtual ~CSysAutoRun();
public:
	typedef struct tagAutoRun{
		std::string		sName;
		std::string		sCommand;
		std::string		sPath;
		std::string		sType;
	}AUTORUN, *PAUTORUN;

public:
	std::vector<AUTORUN>		m_vctAutoRun;

protected:
	void		ScanStartup(std::vector<AUTORUN>& vctRun, int csidl);
	void		ScanRegKey(std::vector<AUTORUN>& vctRun, HKEY hRoot, LPCSTR lpSubPath);

	std::string GetFileCommand(LPCSTR lpPath, LPCSTR lpFile);
	std::string GetRegPath(HKEY hRoot, LPCSTR lpSubPath);
public:
	BOOL		Scan();
	int			GetCount();
};

