// NetShareTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CNetShareTestApp:
// �йش����ʵ�֣������ NetShareTest.cpp
//

class CNetShareTestApp : public CWinApp
{
public:
	CNetShareTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetShareTestApp theApp;
