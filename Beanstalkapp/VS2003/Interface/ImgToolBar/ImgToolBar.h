// ImgToolBar.h : ImgToolBar Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CImgToolBarApp:
// �йش����ʵ�֣������ ImgToolBar.cpp
//

class CImgToolBarApp : public CWinApp
{
public:
	CImgToolBarApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImgToolBarApp theApp;
