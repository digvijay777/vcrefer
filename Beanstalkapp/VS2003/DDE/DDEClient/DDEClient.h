// DDEClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDDEClientApp:
// �йش����ʵ�֣������ DDEClient.cpp
//

class CDDEClientApp : public CWinApp
{
public:
	CDDEClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDDEClientApp theApp;
