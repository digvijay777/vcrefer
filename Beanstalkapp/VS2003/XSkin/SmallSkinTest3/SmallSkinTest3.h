// SmallSkinTest3.h : SmallSkinTest3 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CSmallSkinTest3App:
// �йش����ʵ�֣������ SmallSkinTest3.cpp
//

class CSmallSkinTest3App : public CWinApp
{
public:
	CSmallSkinTest3App();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSmallSkinTest3App theApp;
