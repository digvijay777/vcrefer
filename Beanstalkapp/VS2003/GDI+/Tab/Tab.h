// Tab.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTabApp:
// �йش����ʵ�֣������ Tab.cpp
//

class CTabApp : public CWinApp
{
public:
	CTabApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	ULONG_PTR		m_pGdiToken;
	virtual int ExitInstance();
};

extern CTabApp theApp;
