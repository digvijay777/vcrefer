// GDIDlgTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GDIDlgTest.h"
#include "GDIDlgTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDIDlgTestApp

BEGIN_MESSAGE_MAP(CGDIDlgTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGDIDlgTestApp ����

CGDIDlgTestApp::CGDIDlgTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGDIDlgTestApp ����

CGDIDlgTestApp theApp;


// CGDIDlgTestApp ��ʼ��

BOOL CGDIDlgTestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	m_pGdiToken = NULL;
	GdiplusStartupInput		gdiplus;

	GdiplusStartup(&m_pGdiToken, &gdiplus, NULL);

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

	CGDIDlgTestDlg dlg;
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
	GdiplusShutdown(m_pGdiToken);

	return FALSE;
}
