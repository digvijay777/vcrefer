// JScriptDlg.cpp : ʵ���ļ�
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
END_MESSAGE_MAP()


// CJScriptDlg �Ի���



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


// CJScriptDlg ��Ϣ�������

BOOL CJScriptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	::CoInitialize(NULL);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CJScriptDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CJScriptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CJScriptDlg::OnBnClickedButton1()
{
	// �õ�IIS�����û���Ϣ
	CComQIPtr<IScriptControl>		spScript;
	HRESULT		hr;
	
	hr = spScript.CoCreateInstance(__uuidof(ScriptControl));
	if(spScript == NULL)
	{
		AfxMessageBox("���ܼ��ؽű�����.");
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
