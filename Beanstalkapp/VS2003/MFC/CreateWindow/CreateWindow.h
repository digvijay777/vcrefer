// CreateWindow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCreateWindowApp:
// �йش����ʵ�֣������ CreateWindow.cpp
//

class CCreateWindowApp : public CWinApp
{
public:
	CCreateWindowApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateWindowApp theApp;
