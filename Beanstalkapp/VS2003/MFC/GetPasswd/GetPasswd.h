// GetPasswd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CGetPasswdApp:
// �йش����ʵ�֣������ GetPasswd.cpp
//

class CGetPasswdApp : public CWinApp
{
public:
	CGetPasswdApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGetPasswdApp theApp;
