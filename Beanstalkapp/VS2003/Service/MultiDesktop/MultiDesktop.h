// MultiDesktop.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CMultiDesktopApp:
// �йش����ʵ�֣������ MultiDesktop.cpp
//

class CMultiDesktopApp : public CWinApp
{
public:
	CMultiDesktopApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMultiDesktopApp theApp;
