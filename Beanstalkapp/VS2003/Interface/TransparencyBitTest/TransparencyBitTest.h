// TransparencyBitTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTransparencyBitTestApp:
// �йش����ʵ�֣������ TransparencyBitTest.cpp
//

class CTransparencyBitTestApp : public CWinApp
{
public:
	CTransparencyBitTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTransparencyBitTestApp theApp;
