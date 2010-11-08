// ControlDraw.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "ControlDrawDlg.h"
#include "SysRepairDlg.h"
#include "TreeTestDlg.h"
#include "CheckListBoxDlg.h"
#include "TreeListDlg.h"
#include "CtrlListBoxDlg.h"
#include "FileTreeTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlDrawApp

BEGIN_MESSAGE_MAP(CControlDrawApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CControlDrawApp ����

CControlDrawApp::CControlDrawApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CControlDrawApp ����

CControlDrawApp theApp;


// CControlDrawApp ��ʼ��

BOOL CControlDrawApp::InitInstance()
{
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

// 	CControlDrawDlg dlg;
// 	CSysRepairDlg dlg;
// 	CTreeTestDlg		dlg;
// 	CCheckListBoxDlg	dlg;
// 	CTreeListDlg		dlg;
// 	CCtrlListBoxDlg		dlg;
	CFileTreeTestDlg	dlg;

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
