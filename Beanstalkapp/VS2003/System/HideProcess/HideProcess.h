// HideProcess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CHideProcessApp:
// �йش����ʵ�֣������ HideProcess.cpp
//

class CHideProcessApp : public CWinApp
{
public:
	CHideProcessApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHideProcessApp theApp;
