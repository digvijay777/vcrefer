// SmallSkinTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

#include "../SmallSkin/SmallSkin.h"

// CSmallSkinTestApp:
// �йش����ʵ�֣������ SmallSkinTest.cpp
//

class CSmallSkinTestApp : public CWinApp
{
public:
	CSmallSkinTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSmallSkinTestApp theApp;
