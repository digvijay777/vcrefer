#include "stdafx.h"
#include "SysAutoRun.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

CSysAutoRun::CSysAutoRun()
{
	CoInitialize(NULL);
}

CSysAutoRun::~CSysAutoRun()
{

}

// 扫描
BOOL CSysAutoRun::Scan()
{
	m_vctAutoRun.clear();

	// 扫描运行目录
	ScanStartup(m_vctAutoRun, CSIDL_COMMON_STARTUP);
	ScanStartup(m_vctAutoRun, CSIDL_STARTUP);

	// 扫描注册表启动项
	ScanRegKey(m_vctAutoRun, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	ScanRegKey(m_vctAutoRun, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce");
	ScanRegKey(m_vctAutoRun, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx");
	ScanRegKey(m_vctAutoRun, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServices");
	ScanRegKey(m_vctAutoRun, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServicesOnce");
	ScanRegKey(m_vctAutoRun, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer\\Run");

	ScanRegKey(m_vctAutoRun, HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	ScanRegKey(m_vctAutoRun, HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce");
	ScanRegKey(m_vctAutoRun, HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer\\Run");

	return TRUE;
}

// 得到个数
int CSysAutoRun::GetCount()
{
	return (int)m_vctAutoRun.size();
}

// 扫描运行目录
void CSysAutoRun::ScanStartup(std::vector<AUTORUN>& vctRun, int csidl)
{
	CHAR			szPath[MAX_PATH*2]		= {0};
	CHAR			szStartup[MAX_PATH*2]	= {0};
	BOOL			bSuccess				= FALSE;

	bSuccess = SHGetSpecialFolderPath(NULL, szStartup, csidl, FALSE);
	if(FALSE == bSuccess)
		return ;
	if(('\\' != szStartup[strlen(szStartup) - 1]) || ('/' != szStartup[strlen(szStartup) - 1]))
		strcat(szStartup, "\\");

	HANDLE				hFind;
	WIN32_FIND_DATA		fd			= {0};
	
	strcpy(szPath, szStartup);
	strcat(szPath, "*");
	hFind = FindFirstFile(szPath, &fd);
	do
	{
		if(strcmp(fd.cFileName, ".") == 0
			|| strcmp(fd.cFileName, "..") == 0)
			continue;	// 当前目录
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			continue;	// 隐藏文件

		AUTORUN								run;
		std::vector<AUTORUN>::iterator		itRun;
		
		itRun = vctRun.insert(vctRun.end(), run);
		itRun->sType = "Startup";
		itRun->sName = fd.cFileName;
		itRun->sCommand = GetFileCommand(szStartup, fd.cFileName);
		itRun->sPath = szStartup;
		itRun->sPath += fd.cFileName;
	}while(FindNextFile(hFind, &fd));
	FindClose(hFind);
}

// 得到Link文件的命令行参数
std::string CSysAutoRun::GetFileCommand(LPCSTR lpPath, LPCSTR lpFile)
{
	CHAR				szFilePath[MAX_PATH*2]			= {0};
	CHAR				szArgument[MAX_PATH]			= {0};
	std::string			strRet;
	CHAR*				pLink							= NULL;

	strcpy(szFilePath, lpPath);
	strcat(szFilePath, lpFile);
	pLink = StrRStrI(lpFile, NULL, ".lnk");
	if(NULL == pLink)
		return szFilePath;
	if(stricmp(pLink, ".lnk") != 0)
		return szFilePath;

	// 获取连接中径
	CComPtr<IShellLink>			spShellLink;
	CComPtr<IPersistFile>		spPersisFile;
	HRESULT						hres;

	hres = CoCreateInstance( CLSID_ShellLink, NULL,   CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&spShellLink);
	if(FAILED( hres))   
		return "";
	hres = spShellLink.QueryInterface(&spPersisFile);
	if(FAILED(hres))
		return "";
	spPersisFile->Load(_bstr_t(szFilePath), STGM_READ);
	memset(szFilePath, 0, sizeof(szFilePath));
	spShellLink->GetPath(szFilePath, sizeof(szFilePath), NULL,  SLGP_UNCPRIORITY);
	spShellLink->GetArguments(szArgument, sizeof(szArgument));
	
	if(strlen(szArgument) > 0)
	{
		strRet = "\"";
		strRet += szFilePath;
		strRet += "\" ";
		strRet += szArgument;
	}
	else
	{
		strRet = szFilePath;
	}

	return strRet;
}

// 得到注册表项
void CSysAutoRun::ScanRegKey(std::vector<AUTORUN>& vctRun, HKEY hRoot, LPCSTR lpSubPath)
{
	HKEY			hKey					= NULL;
	CHAR			szName[MAX_PATH]		= {0};
	CHAR			szValue[MAX_PATH]		= {0};
	DWORD			dwValue					= sizeof(szValue);
	DWORD			dwName					= sizeof(szName);
	DWORD			dwType;

	RegOpenKeyA(hRoot, lpSubPath, &hKey);
	if(NULL == hKey)
		return;
	for(DWORD dwIndex = 0
		; ERROR_SUCCESS == RegEnumValueA(hKey, dwIndex, szName, &dwName, NULL, &dwType, (BYTE *)szValue, &dwValue)
		; dwIndex++, dwName = sizeof(szName), dwValue = sizeof(szValue))
	{
		if(0 == szName[0] || 0 == dwName)
			continue;
		if(dwType != REG_SZ)
			continue;
		if(0 == szValue[0] && 0 == dwValue)
			continue;
		if(strnicmp(szValue, "rem ", 4) == 0)
			continue;

		std::vector<AUTORUN>::iterator			itRun;
		AUTORUN									run;

		itRun = vctRun.insert(vctRun.end(), run);
		itRun->sName = szName;
		itRun->sCommand = szValue;
		itRun->sType = GetRegPath(hRoot, lpSubPath);
	}
	RegCloseKey(hKey);
}

std::string CSysAutoRun::GetRegPath(HKEY hRoot, LPCSTR lpSubPath)
{
	std::string		strRet;

	switch((DWORD)hRoot)
	{
	case HKEY_CLASSES_ROOT:
		strRet = "HKCR\\";
		break;
	case HKEY_CURRENT_USER:
		strRet = "HKCU\\";
		break;
	case HKEY_LOCAL_MACHINE:
		strRet = "HKLM\\";
		break;
	case HKEY_USERS:
		strRet = "HKUS\\";
		break;
	case HKEY_CURRENT_CONFIG:
		strRet = "HKCC\\";
		break;
	}

	strRet += lpSubPath;
	return strRet;
}