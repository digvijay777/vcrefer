// GDIDlgTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CGDIDlgTestApp:
// �йش����ʵ�֣������ GDIDlgTest.cpp
//

class CGDIDlgTestApp : public CWinApp
{
public:
	CGDIDlgTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
	ULONG_PTR			m_pGdiToken;

	DECLARE_MESSAGE_MAP()
};

extern CGDIDlgTestApp theApp;
