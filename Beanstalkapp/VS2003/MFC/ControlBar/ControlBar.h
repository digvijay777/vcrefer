// ControlBar.h : ControlBar Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CControlBarApp:
// �йش����ʵ�֣������ ControlBar.cpp
//

class CControlBarApp : public CWinApp
{
public:
	CControlBarApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CControlBarApp theApp;
