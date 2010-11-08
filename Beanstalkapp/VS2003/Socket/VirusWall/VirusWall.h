// VirusWall.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号


// CVirusWallApp:
// 有关此类的实现，请参阅 VirusWall.cpp
//

class CVirusWallApp : public CWinApp
{
public:
	CVirusWallApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现
	
	DECLARE_MESSAGE_MAP()
public:
	CString		m_strUser;
	CString		m_strPwd;
	CString		m_strIP;
	CString		m_strVdbPath;

private:
	int			m_nIDUser;
	int			m_nIDPwd;
	

public:
	BOOL		GetVirusWallUserAndPwd(LPSTR lpIP);
	BOOL		TestLoginVirusWall(LPSTR lpIP, LPSTR lpUser, LPSTR lpPwd);
public:
	BOOL		UploadFile(LPSTR lpIP, LPSTR lpLocalPath, LPSTR lpSvrPath);
	BOOL		UploadDir(LPSTR lpIP, LPSTR lpLocalPath, LPSTR lpSvrPath);
	BOOL		GetVirusList(LPSTR lpIP, std::vector<std::string>& vList);
	BOOL		RestartVirusWallService(LPSTR lpIP);
	BOOL		GetBATFile(LPSTR lpPath);
};

extern CVirusWallApp theApp;
DWORD		MessageWorkLoop(HANDLE hHandle, DWORD dwMilliseconds);
