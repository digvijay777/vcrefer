// CreateDeskLink.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCreateDeskLinkApp:
// �йش����ʵ�֣������ CreateDeskLink.cpp
//

class CCreateDeskLinkApp : public CWinApp
{
public:
	CCreateDeskLinkApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateDeskLinkApp theApp;
