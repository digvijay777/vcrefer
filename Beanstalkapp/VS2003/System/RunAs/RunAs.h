// RunAs.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CRunAsApp:
// �йش����ʵ�֣������ RunAs.cpp
//

class CRunAsApp : public CWinApp
{
public:
	CRunAsApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRunAsApp theApp;
