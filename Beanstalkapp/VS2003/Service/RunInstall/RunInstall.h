// RunInstall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CRunInstallApp:
// �йش����ʵ�֣������ RunInstall.cpp
//

class CRunInstallApp : public CWinApp
{
public:
	CRunInstallApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRunInstallApp theApp;
