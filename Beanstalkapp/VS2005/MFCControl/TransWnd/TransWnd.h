// TransWnd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTransWndApp:
// �йش����ʵ�֣������ TransWnd.cpp
//

class CTransWndApp : public CWinApp
{
public:
	CTransWndApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
private:
	ULONG_PTR			m_pGdiToken;

public:
	DECLARE_MESSAGE_MAP()
};

extern CTransWndApp theApp;