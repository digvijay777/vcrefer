// EHomeLists.h : EHomeLists Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CEHomeListsApp:
// �йش����ʵ�֣������ EHomeLists.cpp
//

class CEHomeListsApp : public CWinApp
{
public:
	CEHomeListsApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEHomeListsApp theApp;