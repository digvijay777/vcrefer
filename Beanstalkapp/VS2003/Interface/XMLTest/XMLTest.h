// XMLTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CXMLTestApp:
// �йش����ʵ�֣������ XMLTest.cpp
//

class CXMLTestApp : public CWinApp
{
public:
	CXMLTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXMLTestApp theApp;
