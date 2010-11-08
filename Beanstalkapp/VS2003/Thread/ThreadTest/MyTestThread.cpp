// MyTestThread.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreadTest.h"
#include "MyTestThread.h"


// CMyTestThread

IMPLEMENT_DYNCREATE(CMyTestThread, CWinThread)

CMyTestThread::CMyTestThread()
{
}

CMyTestThread::~CMyTestThread()
{
}

BOOL CMyTestThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CMyTestThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyTestThread, CWinThread)
	ON_THREAD_MESSAGE(UM_TEST, OnTest)
END_MESSAGE_MAP()


/*
 *	测试消息
 *
 *  添加时间:2009-10-26
 */
 void CMyTestThread::OnTest(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("收到一个线程消息:UM_TEST");
}