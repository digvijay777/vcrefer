// ApartmentTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CApartmentTestApp:
// �йش����ʵ�֣������ ApartmentTest.cpp
//

class CApartmentTestApp : public CWinApp
{
public:
	CApartmentTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CApartmentTestApp theApp;
