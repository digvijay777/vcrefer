// FontTest.h : FontTest Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFontTestApp:
// �йش����ʵ�֣������ FontTest.cpp
//

class CFontTestApp : public CWinApp
{
public:
	CFontTestApp();

private:
	ULONG_PTR		m_pGdiToken;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFontTestApp theApp;