// ScanVirusUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CScanVirusUIApp:
// �йش����ʵ�֣������ ScanVirusUI.cpp
//

class CScanVirusUIApp : public CWinApp
{
public:
	CScanVirusUIApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CScanVirusUIApp theApp;
