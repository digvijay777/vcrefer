// QuickDownload.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CQuickDownloadApp:
// �йش����ʵ�֣������ QuickDownload.cpp
//

class CQuickDownloadApp : public CWinApp
{
public:
	CQuickDownloadApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQuickDownloadApp theApp;
