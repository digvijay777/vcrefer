// RemoteThread.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CRemoteThreadApp:
// �йش����ʵ�֣������ RemoteThread.cpp
//

class CRemoteThreadApp : public CWinApp
{
public:
	CRemoteThreadApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRemoteThreadApp theApp;
