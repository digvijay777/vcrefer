#pragma once
#include <vector>
#include <string>

class CSysCleanTemp
{
public:
	CSysCleanTemp();
	virtual ~CSysCleanTemp();

public:
	enum TempType{CleanTemp_User, CleanTemp_Windows, CleanTemp_Recycle};
	typedef struct tagCleanTemp
	{
		std::string		sPath;
		__int64			nSize;
		TempType		type;
	}CLEANTEMP, *PCLEANTEMP;
private:
	std::vector<CLEANTEMP>		m_vctCleanTemp;
	FILETIME					m_curTime;

	BOOL		ScanDirectory(std::vector<CLEANTEMP>& vctTemp, LPCSTR lpPath, TempType type);
public:
	BOOL		Scan();
	int			GetCount();
	LONG		GetAllSize();
};