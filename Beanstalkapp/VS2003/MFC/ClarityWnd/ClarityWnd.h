// ClarityWnd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CClarityWndApp:
// �йش����ʵ�֣������ ClarityWnd.cpp
//

class CClarityWndApp : public CWinApp
{
public:
	CClarityWndApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CClarityWndApp theApp;
