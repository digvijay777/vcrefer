// ApartmentTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ApartmentTest.h"
#include "ApartmentTestDlg.h"
#include ".\apartmenttestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#import "../SafeArray/SafeArray.tlb" no_namespace named_guids
// CApartmentTestDlg �Ի���



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


// CApartmentTestDlg ��Ϣ�������

BOOL CApartmentTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CApartmentTestDlg::OnPaint() 
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
HCURSOR CApartmentTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
// �߳�
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
	// ���̵߳�Ԫ����
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
