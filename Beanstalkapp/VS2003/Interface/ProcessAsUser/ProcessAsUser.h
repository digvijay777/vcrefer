// ProcessAsUser.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CProcessAsUserApp:
// �йش����ʵ�֣������ ProcessAsUser.cpp
//

class CProcessAsUserApp : public CWinApp
{
public:
	CProcessAsUserApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProcessAsUserApp theApp;
