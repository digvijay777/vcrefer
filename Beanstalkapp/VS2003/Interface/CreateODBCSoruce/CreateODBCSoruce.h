// CreateODBCSoruce.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCreateODBCSoruceApp:
// �йش����ʵ�֣������ CreateODBCSoruce.cpp
//

class CCreateODBCSoruceApp : public CWinApp
{
public:
	CCreateODBCSoruceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateODBCSoruceApp theApp;
