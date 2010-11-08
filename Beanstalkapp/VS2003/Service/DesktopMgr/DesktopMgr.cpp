// DesktopMgr.cpp : 定义应用程序的类行为。
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


// CDesktopMgrApp 构造

CDesktopMgrApp::CDesktopMgrApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDesktopMgrApp 对象

CDesktopMgrApp theApp;


// CDesktopMgrApp 初始化

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
