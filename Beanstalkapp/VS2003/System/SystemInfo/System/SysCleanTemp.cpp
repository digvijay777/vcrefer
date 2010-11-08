#include "stdafx.h"
#include "SysCleanTemp.h"

CSysCleanTemp::CSysCleanTemp()
{
	SYSTEMTIME		sysTime		= {0};

	GetSystemTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &m_curTime);
}

CSysCleanTemp::~CSysCleanTemp()
{

}

BOOL CSysCleanTemp::Scan()
{
	m_vctCleanTemp.clear();

	CHAR		szTemp[MAX_PATH*2]		= {0};

	// 扫描用户临时目录
	GetTempPath(sizeof(szTemp), szTemp);
	ScanDirectory(m_vctCleanTemp, szTemp, CleanTemp_User);

// 	// 扫描Windows临时目录
// 	GetTempPath(sizeof(szTemp), szTemp);
// 	ScanDirectory(m_vctCleanTemp, szTemp);

	// 扫描回收站
	
// 	ScanDirectory(m_vctCleanTemp, szTemp);

	return TRUE;
}

int CSysCleanTemp::GetCount()
{
	return (int)m_vctCleanTemp.size();
}

BOOL CSysCleanTemp::ScanDirectory(std::vector<CLEANTEMP>& vctTemp, LPCSTR lpPath, TempType type)
{
	std::string			strPath;
	std::string			strSubPath;

	strPath = lpPath;
	strPath += "\\*.*";

	HANDLE				hFind;
	WIN32_FIND_DATA		fd			= {0};
	__int64				i64Time;

	hFind = FindFirstFile(strPath.c_str(), &fd);
	if(INVALID_HANDLE_VALUE == hFind)
		return FALSE;
	while(FindNextFile(hFind, &fd))
	{
		if(stricmp(fd.cFileName, ".") == 0
			|| stricmp(fd.cFileName, "..") == 0)
			continue;
		strSubPath = lpPath;
		strSubPath += fd.cFileName;
		// 判断目录
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ScanDirectory(vctTemp, strSubPath.c_str(), type);
			continue;
		}
		// 判断文件
		if((__int64&)fd.ftLastWriteTime > (__int64&)fd.ftCreationTime)
			i64Time = (__int64&)m_curTime - (__int64&)fd.ftLastWriteTime;
		else 
			i64Time = (__int64&)m_curTime - (__int64&)fd.ftCreationTime;

		if(i64Time < 0)
			i64Time = 0 - i64Time;
		if(i64Time < Int32x32To64(24, 3600) * (__int64)10000000)
			continue;	// 小于一天就不操作
		// 确认为垃圾
		CLEANTEMP				clean;
		std::vector<CLEANTEMP>::iterator		itClean;

		itClean = vctTemp.insert(vctTemp.end(), clean);
		itClean->sPath = strSubPath;
		itClean->type = type;
		itClean->nSize = ((__int64)fd.nFileSizeHigh << 32) + (__int64)fd.nFileSizeLow;
	}
	FindClose(hFind);

	return TRUE;
}

LONG CSysCleanTemp::GetAllSize()
{
	LONG		nRetSize		= 0;
	__int64		nSize			= 0;

	for(int i = 0; i < m_vctCleanTemp.size(); i++)
	{
		nSize += m_vctCleanTemp[i].nSize;
	}

	nRetSize = nSize / 1024 / 1024;
	return nRetSize;
}