// PlayGIF.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "PlayGIF.h"
#include "PlayGIFDlg.h"
#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>
#include ".\playgif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "gdiplus.lib")

// CPlayGIFApp

BEGIN_MESSAGE_MAP(CPlayGIFApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPlayGIFApp ����

CPlayGIFApp::CPlayGIFApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pGdiToken = NULL;
}


// Ψһ��һ�� CPlayGIFApp ����

CPlayGIFApp theApp;


// CPlayGIFApp ��ʼ��

BOOL CPlayGIFApp::InitInstance()
{
	// GDI+��
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &gdiplusStartupInput, NULL);
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CPlayGIFDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CPlayGIFApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(m_pGdiToken);	

	return CWinApp::ExitInstance();
}
