// XSkinDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CXSkinDlgApp:
// �йش����ʵ�֣������ XSkinDlg.cpp
//

class CXSkinDlgApp : public CWinApp
{
public:
	CXSkinDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXSkinDlgApp theApp;
