// LogToolTest.h : LogToolTest Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CLogToolTestApp:
// �йش����ʵ�֣������ LogToolTest.cpp
//

class CLogToolTestApp : public CWinApp
{
public:
	CLogToolTestApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLogToolTestApp theApp;
