// SaveCom.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSaveComApp:
// �йش����ʵ�֣������ SaveCom.cpp
//

class CSaveComApp : public CWinApp
{
public:
	CSaveComApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSaveComApp theApp;
