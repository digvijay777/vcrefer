// Detours.h : Detours Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������
//////////////////////////////////////////////////////////////////////////
// �����ⲿ��
#include "../../../common/mfc/TabToolBar.h"
#include "../../../common/Detours/detours.h"

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֱ���
extern class CDetoursApp theApp;


//////////////////////////////////////////////////////////////////////////
// �Զ�����Ϣ��
#define UM_FRAME			(WM_USER+100)
#define UM_DOCUMENT			(WM_USER+200)
#define UM_VIEW				(WM_USER+300)

#define UM_FRAME_UPDATETABLE		UM_FRAME+1

#define UM_VIEW_ADDITEM				UM_VIEW+1
//////////////////////////////////////////////////////////////////////////
// ������
#define COMPAREINCLUDE(mask, in)	(((mask) & (in)) == (in))

//////////////////////////////////////////////////////////////////////////
// APP��
class CDetoursApp : public CWinApp
{
public:
	CDetoursApp();


	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};