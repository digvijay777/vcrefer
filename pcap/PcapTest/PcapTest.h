// PcapTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CPcapTestApp:
// �йش����ʵ�֣������ PcapTest.cpp
//

class CPcapTestApp : public CWinApp
{
public:
	CPcapTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPcapTestApp theApp;
