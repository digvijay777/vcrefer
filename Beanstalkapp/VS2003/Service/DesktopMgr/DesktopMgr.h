// DesktopMgr.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号


// CDesktopMgrApp:
// 有关此类的实现，请参阅 DesktopMgr.cpp
//

class CDesktopMgrApp : public CWinApp
{
public:
	CDesktopMgrApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

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
