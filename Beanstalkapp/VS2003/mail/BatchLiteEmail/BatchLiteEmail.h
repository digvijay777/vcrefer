// BatchLiteEmail.h : BatchLiteEmail Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CBatchLiteEmailApp:
// �йش����ʵ�֣������ BatchLiteEmail.cpp
//

class CBatchLiteEmailApp : public CWinApp
{
public:
	CBatchLiteEmailApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	CString GetFirstRecentFile();
};

extern CBatchLiteEmailApp theApp;

CString	GetHistory(LPCTSTR lpKey, LPCTSTR lpDefault = NULL);
void SetHistory(LPCTSTR lpKey, LPCTSTR lpValue);

