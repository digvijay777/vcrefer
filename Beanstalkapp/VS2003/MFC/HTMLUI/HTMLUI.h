// HTMLUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CHTMLUIApp:
// �йش����ʵ�֣������ HTMLUI.cpp
//

class CHTMLUIApp : public CWinApp
{
public:
	CHTMLUIApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHTMLUIApp theApp;
