// NoManifest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNoManifestApp:
// �йش����ʵ�֣������ NoManifest.cpp
//

class CNoManifestApp : public CWinApp
{
public:
	CNoManifestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNoManifestApp theApp;