// NetBIOSTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CNetBIOSTestApp:
// �йش����ʵ�֣������ NetBIOSTest.cpp
//

class CNetBIOSTestApp : public CWinApp
{
public:
	CNetBIOSTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetBIOSTestApp theApp;
