// MyTestThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CMyTestThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyTestThread, CWinThread)
	ON_THREAD_MESSAGE(UM_TEST, OnTest)
END_MESSAGE_MAP()


/*
 *	������Ϣ
 *
 *  ���ʱ��:2009-10-26
 */
 void CMyTestThread::OnTest(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("�յ�һ���߳���Ϣ:UM_TEST");
}