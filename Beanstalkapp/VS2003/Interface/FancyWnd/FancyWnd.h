// FancyWnd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFancyWndApp:
// �йش����ʵ�֣������ FancyWnd.cpp
//

class CFancyWndApp : public CWinApp
{
public:
	CFancyWndApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFancyWndApp theApp;
