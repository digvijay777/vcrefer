// ThreadTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CThreadTestApp:
// �йش����ʵ�֣������ ThreadTest.cpp
//

class CThreadTestApp : public CWinApp
{
public:
	CThreadTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CThreadTestApp theApp;
