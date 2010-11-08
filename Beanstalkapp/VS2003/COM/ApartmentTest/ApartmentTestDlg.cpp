// ApartmentTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ApartmentTest.h"
#include "ApartmentTestDlg.h"
#include ".\apartmenttestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#import "../SafeArray/SafeArray.tlb" no_namespace named_guids
// CApartmentTestDlg 对话框



CApartmentTestDlg::CApartmentTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApartmentTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApartmentTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CApartmentTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CApartmentTestDlg 消息处理程序

BOOL CApartmentTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CApartmentTestDlg::OnPaint() 
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
HCURSOR CApartmentTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
// 线程
DWORD WINAPI CApartmentTestDlg::STAThreadProc(LPVOID lpParameter)
{
// 	ISTAtest*		pSta	= (ISTAtest *)lpParameter;
// 
// 	long	nID		= pSta->GetID();
// 	TRACE("ISTAtest Thread 1 ID:%ld\n", nID);
	
	HANDLE			hEvet	= (HANDLE)lpParameter;

	long	nID;
	ISTAtest*		pSta2	= NULL;
	HRESULT			hres	= 0;

	hres = CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_STAtest, NULL, CLSCTX_INPROC_SERVER, IID_ISTAtest, (VOID **)&pSta2);

	nID = pSta2->GetID();
	pSta2->Release();
	TRACE("ISTAtest Thread 2 ID:%ld\n", nID);
//	CoUninitialize();
	SetEvent(hEvet);
	Sleep(10000);
// 	WaitForSingleObject(hEvet, INFINITE);
	return 0;
}
void CApartmentTestDlg::OnBnClickedButton1()
{
	HANDLE		hEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE		hThread		= CreateThread(NULL, 0, STAThreadProc, (void *)hEvent, 0, 0);
//	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hEvent, INFINITE);

// 	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoInitialize(NULL);

// 	SetEvent(hEvent);
// 	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread);

	HRESULT			hres	= 0;
	ISTAtest*		pSta1	= NULL;
// 	ISTAtest*		pSta2	= NULL;
	hres = CoCreateInstance(CLSID_STAtest, NULL, CLSCTX_INPROC_SERVER, IID_ISTAtest, (VOID **)&pSta1);
	
	long		nID		= pSta1->GetID();
	pSta1->Release();
	CoUninitialize();
	TRACE("ISTAtest Thread 1 ID:%ld\n", nID);
	// 单线程单元测试
}

DWORD CALLBACK MulThreadReadTest(LPVOID lpParameter)
{
	CoInitialize(NULL);
	ISTAtest*		pSta1		= (ISTAtest *)lpParameter;

	long		nID		= pSta1->GetID();
	TRACE("ISTAtest MulThreadRead 1 ID: %ld\n", nID);
	CoUninitialize();
	return 0;
}
void CApartmentTestDlg::OnBnClickedButton2()
{
	CoInitialize(NULL);
	ISTAtest*			pSta1		= NULL;
	HRESULT				hres		= E_FAIL;
	HANDLE				hThread		= NULL;

	hres = CoCreateInstance(CLSID_STAtest, NULL, CLSCTX_INPROC_SERVER, IID_ISTAtest, (void **)&pSta1);
	if(FAILED(hres))
		return;

	hThread = CreateThread(NULL, 0, MulThreadReadTest, pSta1, 0, NULL);
	// Sleep(10000);

	long		nID		= pSta1->GetID();
	TRACE("ISTAtest MulThreadRead 2 ID: %ld\n", nID);
	WaitForSingleObject(hThread, INFINITE);
		
	CoUninitialize();
}
