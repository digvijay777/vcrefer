// SkinWnd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSkinWndApp:
// �йش����ʵ�֣������ SkinWnd.cpp
//

class CSkinWndApp : public CWinApp
{
public:
	CSkinWndApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSkinWndApp theApp;