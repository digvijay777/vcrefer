// RemoteThreadDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CRemoteThreadDlgApp:
// �йش����ʵ�֣������ RemoteThreadDlg.cpp
//

class CRemoteThreadDlgApp : public CWinApp
{
public:
	CRemoteThreadDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRemoteThreadDlgApp theApp;
