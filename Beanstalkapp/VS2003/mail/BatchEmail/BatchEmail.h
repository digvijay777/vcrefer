// BatchEmail.h : BatchEmail Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CBatchEmailApp:
// �йش����ʵ�֣������ BatchEmail.cpp
//

class CBatchEmailApp : public CWinApp
{
public:
	CBatchEmailApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBatchEmailApp theApp;
