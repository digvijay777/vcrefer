// JScriptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JScript.h"
#include "JScriptDlg.h"
#include ".\jscriptdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#import "msscript.ocx" no_namespace

#include "script.h"
#include <atlsafe.h>
#pragma comment(lib, "comsuppw.lib")
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
END_MESSAGE_MAP()


// CJScriptDlg 对话框



CJScriptDlg::CJScriptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJScriptDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJScriptDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CJScriptDlg 消息处理程序

BOOL CJScriptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	::CoInitialize(NULL);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CJScriptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJScriptDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CJScriptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CJScriptDlg::OnBnClickedButton1()
{
	// 得到IIS匿名用户信息
	CComQIPtr<IScriptControl>		spScript;
	HRESULT		hr;
	
	hr = spScript.CoCreateInstance(__uuidof(ScriptControl));
	if(spScript == NULL)
	{
		AfxMessageBox("不能加载脚本引擎.");
		return;
	}

	spScript->Timeout = 1000 * 2;
	spScript->PutAllowUI(VARIANT_FALSE);
	spScript->PutLanguage(L"JScript");
	spScript->AddCode(JScript_GetIIS);

	VARIANT va = {0}; 
	CComSafeArray<VARIANT>	spArray;
	spArray.Add(va);
	try
	{
// 		CComPtr < IScriptProcedureCollection > aProcedureSet = m_iScriptControl -> GetProcedures (); 
// 		long n = aProcedureSet -> GetCount (); 
// 		CComPtr < IScriptProcedure > aProcedure = aProcedureSet -> GetItem ( _variant_t ( n )); 
// 		_bstr_t strFunction = aProcedure -> GetName (); 
	
		_variant_t varRet = spScript->Run("GetIIS", spArray.GetSafeArrayPtr());
		AfxMessageBox(_bstr_t(varRet.bstrVal));
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

void CJScriptDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

void CJScriptDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CJScriptDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	::CoUninitialize();
	CDialog::OnCancel();
}

#include "study.h"

void CJScriptDlg::OnBnClickedButton2()
{
	CScriptingSupportHelper		script;
	
	script.Create(this);
	script.RunScript(/*JScript_Alert*/JScript_GetIIS);
	CComQIPtr<IScriptControl>		spScript;
	script.RunScript("GetIIS();");
	HRESULT hr = script.m_pActiveScriptParse->QueryInterface(&spScript);
//	script.m_pActiveScript->QueryInterface();
	if(spScript != NULL)
	{
		VARIANT va = {0}; 
		CComSafeArray<VARIANT>	spArray;
		spArray.Add(va);
		try
		{
			_variant_t varRet = spScript->Run("GetIIS", spArray.GetSafeArrayPtr());
			AfxMessageBox(_bstr_t(varRet.bstrVal));
		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());
		}
	}
}
