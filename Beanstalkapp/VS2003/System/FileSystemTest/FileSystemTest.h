// FileSystemTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFileSystemTestApp:
// �йش����ʵ�֣������ FileSystemTest.cpp
//

class CFileSystemTestApp : public CWinApp
{
public:
	CFileSystemTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFileSystemTestApp theApp;
