// DesktopMgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDesktopMgrApp:
// �йش����ʵ�֣������ DesktopMgr.cpp
//

class CDesktopMgrApp : public CWinApp
{
public:
	CDesktopMgrApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int Run();
public:
	HHOOK		m_hook				= NULL;
	DWORD		m_dwMainThreadID	= 0;
	HDESK		m_hDeskDown			= NULL;
	HDESK		m_hDeskLeft			= NULL;
	HDESK		m_hDeskRight		= NULL;
	HDESK		m_hDeskUp			= NULL;
};

extern CDesktopMgrApp theApp;
