// ExecuteSQL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CExecuteSQLApp:
// �йش����ʵ�֣������ ExecuteSQL.cpp
//

class CExecuteSQLApp : public CWinApp
{
public:
	CExecuteSQLApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExecuteSQLApp theApp;
