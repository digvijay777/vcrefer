// testsdk.h : testsdk Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CtestsdkApp:
// �йش����ʵ�֣������ testsdk.cpp
//

class CtestsdkApp : public CWinApp
{
public:
	CtestsdkApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CtestsdkApp theApp;
