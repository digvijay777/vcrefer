// GDIPDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CGDIPDlgApp:
// �йش����ʵ�֣������ GDIPDlg.cpp
//

class CGDIPDlgApp : public CWinApp
{
public:
	CGDIPDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGDIPDlgApp theApp;
