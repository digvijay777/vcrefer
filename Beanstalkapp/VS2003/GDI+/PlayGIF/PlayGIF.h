// PlayGIF.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CPlayGIFApp:
// �йش����ʵ�֣������ PlayGIF.cpp
//

class CPlayGIFApp : public CWinApp
{
public:
	CPlayGIFApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
	ULONG_PTR		m_pGdiToken;
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CPlayGIFApp theApp;
