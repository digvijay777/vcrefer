// LeakCheck.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CLeakCheckApp:
// �йش����ʵ�֣������ LeakCheck.cpp
//

class CLeakCheckApp : public CWinApp
{
public:
	CLeakCheckApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLeakCheckApp theApp;
