// VirusWall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CVirusWallApp:
// �йش����ʵ�֣������ VirusWall.cpp
//

class CVirusWallApp : public CWinApp
{
public:
	CVirusWallApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
	
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
