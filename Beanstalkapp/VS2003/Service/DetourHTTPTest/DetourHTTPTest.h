// DetourHTTPTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDetourHTTPTestApp:
// �йش����ʵ�֣������ DetourHTTPTest.cpp
//

class CDetourHTTPTestApp : public CWinApp
{
public:
	CDetourHTTPTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDetourHTTPTestApp theApp;
