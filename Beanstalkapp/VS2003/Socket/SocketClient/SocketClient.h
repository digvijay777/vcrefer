// SocketClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSocketClientApp:
// �йش����ʵ�֣������ SocketClient.cpp
//

class CSocketClientApp : public CWinApp
{
public:
	CSocketClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSocketClientApp theApp;
