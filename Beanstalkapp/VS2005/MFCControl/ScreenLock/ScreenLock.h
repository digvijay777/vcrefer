// ScreenLock.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SwitchDesktop.h"


// CScreenLockApp:
// �йش����ʵ�֣������ ScreenLock.cpp
//

class CScreenLockApp : public CWinApp
{
public:
	CScreenLockApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	CSwitchDesktop	m_desktop;
};

extern CScreenLockApp theApp;