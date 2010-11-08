// LicenseManager.h : LicenseManager 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF")

// CLicenseManagerApp:
// 有关此类的实现，请参阅 LicenseManager.cpp
//

class CLicenseManagerApp : public CWinApp
{
public:
	CLicenseManagerApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
private:
	TCHAR		m_szAppPath[MAX_PATH];
	TCHAR		m_szIniPath[MAX_PATH];
	TCHAR		m_szSQLString[1024];
private:
	LPCTSTR		GetSQLConnectStr();
public:
	LPCTSTR		GetAppPath();
	LPCTSTR		GetIniPath();
public:
// 	_ConnectionPtr		m_spConnect;
	virtual int ExitInstance();
};

extern CLicenseManagerApp theApp;

//////////////////////////////////////////////////////////////////////////
// 工程中所用到的消息列表
#define UM_FRAMEMESSAGE					(WM_USER+100)
#define UM_UPDATESEPARATOR				UM_FRAMEMESSAGE+1		//　更新状态栏

#define UM_DOCMESSAGE					(WM_USER+200)			
#define UM_UPDATESHOW					UM_DOCMESSAGE+1			// 更新LIST控件

#define UM_GETKEYVALIDATE				UM_DOCMESSAGE+51		// 得到License的有效性
