// DDEServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDDEServerApp:
// �йش����ʵ�֣������ DDEServer.cpp
//

class CDDEServerApp : public CWinApp
{
public:
	CDDEServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDDEServerApp theApp;
