// ProxySocks.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CProxySocksApp:
// �йش����ʵ�֣������ ProxySocks.cpp
//

class CProxySocksApp : public CWinApp
{
public:
	CProxySocksApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProxySocksApp theApp;
