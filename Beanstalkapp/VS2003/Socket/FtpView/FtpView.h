// FtpView.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFtpViewApp:
// �йش����ʵ�֣������ FtpView.cpp
//

class CFtpViewApp : public CWinApp
{
public:
	CFtpViewApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFtpViewApp theApp;
