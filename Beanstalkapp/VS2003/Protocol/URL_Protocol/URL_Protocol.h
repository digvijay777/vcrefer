// URL_Protocol.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CURL_ProtocolApp:
// �йش����ʵ�֣������ URL_Protocol.cpp
//

class CURL_ProtocolApp : public CWinApp
{
public:
	CURL_ProtocolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CURL_ProtocolApp theApp;
