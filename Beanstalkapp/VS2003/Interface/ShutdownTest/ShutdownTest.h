// ShutdownTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CShutdownTestApp:
// �йش����ʵ�֣������ ShutdownTest.cpp
//

class CShutdownTestApp : public CWinApp
{
public:
	CShutdownTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CShutdownTestApp theApp;
