// DynamicResource.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDynamicResourceApp:
// �йش����ʵ�֣������ DynamicResource.cpp
//

class CDynamicResourceApp : public CWinApp
{
public:
	CDynamicResourceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDynamicResourceApp theApp;
