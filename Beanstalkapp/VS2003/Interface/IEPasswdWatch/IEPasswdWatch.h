// IEPasswdWatch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CIEPasswdWatchApp:
// �йش����ʵ�֣������ IEPasswdWatch.cpp
//

class CIEPasswdWatchApp : public CWinApp
{
public:
	CIEPasswdWatchApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIEPasswdWatchApp theApp;
