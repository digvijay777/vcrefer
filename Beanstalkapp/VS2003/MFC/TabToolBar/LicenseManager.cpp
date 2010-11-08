// LicenseManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "MainFrm.h"

#include "LicenseManagerDoc.h"
#include "License2View.h"
#include ".\licensemanager.h"
#include "TabCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "comsuppw.lib")
// CLicenseManagerApp

BEGIN_MESSAGE_MAP(CLicenseManagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CLicenseManagerApp ����

CLicenseManagerApp::CLicenseManagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	memset(m_szAppPath, 0, MAX_PATH * sizeof(TCHAR));
	memset(m_szIniPath, 0, MAX_PATH * sizeof(TCHAR));
	memset(m_szSQLString, 0, 1024 * sizeof(TCHAR));
}


// Ψһ��һ�� CLicenseManagerApp ����

CLicenseManagerApp theApp;

// CLicenseManagerApp ��ʼ��

BOOL CLicenseManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	CTabCtrlDlg		dlg;

	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
	//�������ݿ�
// 	try{
// 		m_spConnect.CreateInstance(__uuidof(Connection));
// 		m_spConnect->Open(_bstr_t(GetSQLConnectStr()), "", "", 0);
// 	}
// 	catch(_com_error e)
// 	{
// 		AfxMessageBox(e.Description(), MB_OK | MB_ICONERROR);
// 		return FALSE;
// 	}

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLicenseManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CLicense2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CLicenseManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CLicenseManagerApp ��Ϣ�������
LPCTSTR CLicenseManagerApp::GetSQLConnectStr()
{
	if(0 != m_szSQLString[0])
		return m_szSQLString;

	TCHAR		szServer[200]		= {0};
	TCHAR		szUser[200]			= {0};
	TCHAR		szPasswd[200]		= {0};
	TCHAR		szData[200]			= {0};
	
	::GetPrivateProfileString("SQL", "Server", "www.xmtoc.com", szServer, 200, GetIniPath());
	::GetPrivateProfileString("SQL", "User", "TcLicense", szUser, 200, GetIniPath());
	::GetPrivateProfileString("SQL", "Passwd", ")(xmtc87", szPasswd, 200, GetIniPath());
	::GetPrivateProfileString("SQL", "DataBase", "License", szData, 200, GetIniPath());
	_stprintf(m_szSQLString, "Provider=SQLOLEDB.1;User ID=%s;Password=%s;Persist Security Info=True;Initial Catalog=%s;Network Address=%s", 
		szUser, szPasswd, szData, szServer);
	return m_szSQLString;
}

LPCTSTR CLicenseManagerApp::GetAppPath()
{
	if(0 != m_szAppPath[0])
		return m_szAppPath;

	TCHAR*		pChar					= NULL;
    GetModuleFileName(AfxGetInstanceHandle(), m_szAppPath, MAX_PATH);
	pChar = strrchr(m_szAppPath, '\\');
	if(NULL != pChar)
		*(pChar + 1) = 0;
	return m_szAppPath;
}
LPCTSTR CLicenseManagerApp::GetIniPath()
{
	if(0 != m_szIniPath[0])
		return m_szIniPath;

	strcpy(m_szIniPath, GetAppPath());
	strcat(m_szIniPath, "Config.ini");
	return m_szIniPath;
}
int CLicenseManagerApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
// 	long lState = m_spConnect->GetState();
// 	if(0 != lState)
// 		m_spConnect->Close();

	return CWinApp::ExitInstance();
}
