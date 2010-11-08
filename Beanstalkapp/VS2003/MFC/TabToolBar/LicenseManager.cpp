// LicenseManager.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CLicenseManagerApp 构造

CLicenseManagerApp::CLicenseManagerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	memset(m_szAppPath, 0, MAX_PATH * sizeof(TCHAR));
	memset(m_szIniPath, 0, MAX_PATH * sizeof(TCHAR));
	memset(m_szSQLString, 0, 1024 * sizeof(TCHAR));
}


// 唯一的一个 CLicenseManagerApp 对象

CLicenseManagerApp theApp;

// CLicenseManagerApp 初始化

BOOL CLicenseManagerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	CTabCtrlDlg		dlg;

	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
	//连接数据库
// 	try{
// 		m_spConnect.CreateInstance(__uuidof(Connection));
// 		m_spConnect->Open(_bstr_t(GetSQLConnectStr()), "", "", 0);
// 	}
// 	catch(_com_error e)
// 	{
// 		AfxMessageBox(e.Description(), MB_OK | MB_ICONERROR);
// 		return FALSE;
// 	}

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLicenseManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CLicense2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CLicenseManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CLicenseManagerApp 消息处理程序
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
