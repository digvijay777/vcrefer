// TestProxy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTestProxyApp:
// �йش����ʵ�֣������ TestProxy.cpp
//

class CTestProxyApp : public CWinApp
{
public:
	CTestProxyApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestProxyApp theApp;
