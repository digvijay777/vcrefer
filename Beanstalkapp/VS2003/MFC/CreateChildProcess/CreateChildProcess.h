// CreateChildProcess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCreateChildProcessApp:
// �йش����ʵ�֣������ CreateChildProcess.cpp
//

class CCreateChildProcessApp : public CWinApp
{
public:
	CCreateChildProcessApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateChildProcessApp theApp;
