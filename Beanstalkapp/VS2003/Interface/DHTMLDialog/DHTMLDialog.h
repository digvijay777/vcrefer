// DHTMLDialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDHTMLDialogApp:
// �йش����ʵ�֣������ DHTMLDialog.cpp
//

class CDHTMLDialogApp : public CWinApp
{
public:
	CDHTMLDialogApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDHTMLDialogApp theApp;
