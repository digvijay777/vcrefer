// UpnpManger.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CUpnpMangerApp:
// �йش����ʵ�֣������ UpnpManger.cpp
//

class CUpnpMangerApp : public CWinApp
{
public:
	CUpnpMangerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUpnpMangerApp theApp;
