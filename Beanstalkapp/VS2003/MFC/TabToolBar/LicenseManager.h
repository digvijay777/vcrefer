// LicenseManager.h : LicenseManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF")

// CLicenseManagerApp:
// �йش����ʵ�֣������ LicenseManager.cpp
//

class CLicenseManagerApp : public CWinApp
{
public:
	CLicenseManagerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
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
// ���������õ�����Ϣ�б�
#define UM_FRAMEMESSAGE					(WM_USER+100)
#define UM_UPDATESEPARATOR				UM_FRAMEMESSAGE+1		//������״̬��

#define UM_DOCMESSAGE					(WM_USER+200)			
#define UM_UPDATESHOW					UM_DOCMESSAGE+1			// ����LIST�ؼ�

#define UM_GETKEYVALIDATE				UM_DOCMESSAGE+51		// �õ�License����Ч��
