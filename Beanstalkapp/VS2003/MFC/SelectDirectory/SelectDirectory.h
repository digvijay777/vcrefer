// SelectDirectory.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSelectDirectoryApp:
// �йش����ʵ�֣������ SelectDirectory.cpp
//

class CSelectDirectoryApp : public CWinApp
{
public:
	CSelectDirectoryApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSelectDirectoryApp theApp;
