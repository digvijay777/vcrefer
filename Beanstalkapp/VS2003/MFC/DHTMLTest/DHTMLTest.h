// DHTMLTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDHTMLTestApp:
// �йش����ʵ�֣������ DHTMLTest.cpp
//

class CDHTMLTestApp : public CWinApp
{
public:
	CDHTMLTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDHTMLTestApp theApp;
