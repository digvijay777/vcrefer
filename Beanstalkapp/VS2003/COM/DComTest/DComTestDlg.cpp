// DComTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DComTest.h"
#include "DComTestDlg.h"
#include ".\dcomtestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CDComTestDlg �Ի���



CDComTestDlg::CDComTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDComTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDComTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDComTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDComTestDlg ��Ϣ�������

BOOL CDComTestDlg::OnInitDialog()
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
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CDComTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDComTestDlg::OnPaint() 
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
HCURSOR CDComTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDComTestDlg::OnBnClickedButton1()
{
	// �õ�Զ��������
	::CoInitialize(NULL);
	
	CLSID		clsid;
	HRESULT		hres;

	hres = ::CLSIDFromProgID(OLESTR("DCOMServer.FirstDCOM"), &clsid);
	
	LPUNKNOWN	punk;
	hres = ::CoCreateInstance(clsid, NULL, CLSCTX_SERVER,
		IID_IUnknown, (LPVOID FAR *)&punk);

	if(FAILED(hres))
	{
		AfxMessageBox("�õ��ӿ�ʧ��");
		::CoUninitialize();
		return ;
	}
	
	LPDISPATCH		pdisp = NULL;

	hres = punk->QueryInterface(IID_IDispatch, (LPVOID FAR *)&pdisp);

	if(FAILED(hres) || NULL == pdisp)
	{
		punk->Release();
		AfxMessageBox("Get Dispatch Failed.");
		::CoUninitialize();
	}

	hres = punk->Release();

	DISPID	dispid;
	CComBSTR	bstr = "GetServerName";
	hres = pdisp->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_USER_DEFAULT, &dispid);
	if(SUCCEEDED(hres))
	{
		DISPPARAMS		dispparams	= {0};
		VARIANTARG		var;

		dispparams.cArgs = 0;
// 		dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 		dispparams.rgvarg[0].vt = VT_BYREF|VT_BSTR ;
// 		dispparams.rgvarg[0].pbstrVal = &bstrRet;
		VariantInit(&var);

		hres = pdisp->Invoke(dispid, IID_NULL, 
			LOCALE_USER_DEFAULT, DISPATCH_METHOD,
			&dispparams, &var, NULL, NULL);

		if(SUCCEEDED(hres))
		{
			::MessageBoxW(m_hWnd, var.bstrVal, L"DComReturn", MB_OK | MB_ICONINFORMATION);
			::SysFreeString(var.bstrVal);
		}
	}

	pdisp->Release();
	::CoUninitialize();
}
