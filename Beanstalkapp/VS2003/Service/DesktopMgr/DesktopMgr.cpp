// DesktopMgr.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DesktopMgr.h"
#include "DesktopMgrDlg.h"
#include ".\desktopmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDesktopMgrApp

BEGIN_MESSAGE_MAP(CDesktopMgrApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDesktopMgrApp ����

CDesktopMgrApp::CDesktopMgrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDesktopMgrApp ����

CDesktopMgrApp theApp;


// CDesktopMgrApp ��ʼ��

BOOL CDesktopMgrApp::InitInstance()
{
	InitCommonControls();
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	return TRUE;
}

int CDesktopMgrApp::Run()
{
	
	return 1;
}
