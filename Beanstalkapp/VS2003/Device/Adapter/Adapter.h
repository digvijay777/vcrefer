// Adapter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CAdapterApp:
// �йش����ʵ�֣������ Adapter.cpp
//

class CAdapterApp : public CWinApp
{
public:
	CAdapterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAdapterApp theApp;
