// SmallSkinTest2.h : SmallSkinTest2 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CSmallSkinTest2App:
// �йش����ʵ�֣������ SmallSkinTest2.cpp
//

class CSmallSkinTest2App : public CWinApp
{
public:
	CSmallSkinTest2App();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSmallSkinTest2App theApp;
