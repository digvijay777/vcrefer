// CreateRGB.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCreateRGBApp:
// �йش����ʵ�֣������ CreateRGB.cpp
//

class CCreateRGBApp : public CWinApp
{
public:
	CCreateRGBApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateRGBApp theApp;
