// StudyGDI+.h : StudyGDI+ Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CStudyGDIApp:
// �йش����ʵ�֣������ StudyGDI+.cpp
//

class CStudyGDIApp : public CWinApp
{
public:
	CStudyGDIApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStudyGDIApp theApp;
