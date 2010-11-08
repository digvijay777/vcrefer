// CommonTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CommonTest.h"
#include "CommonTestDlg.h"
#include ".\commontestdlg.h"

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


// CCommonTestDlg �Ի���



CCommonTestDlg::CCommonTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommonTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCommonTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCommonTestDlg ��Ϣ�������

BOOL CCommonTestDlg::OnInitDialog()
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

void CCommonTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommonTestDlg::OnPaint() 
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
HCURSOR CCommonTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCommonTestDlg::OnBnClickedButton1()
{
	// COM ERROR TEST
	CoInitialize(NULL);

	CLSID			iid;
	HRESULT			hres;

// 	hres = CLSIDFromProgID(OLESTR("SafeArray.ErrTest2"), &iid);
	hres = CLSIDFromProgID(OLESTR("SafeArray.ErrTest"), &iid);
	
	if(FAILED(hres))
	{
		CoUninitialize();
		return;
	}

	LPUNKNOWN		pUkn	= NULL;
	hres = CoCreateInstance(iid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&pUkn);
	if(FAILED(hres))
	{
		CoUninitialize();
		return;
	}

	LPDISPATCH		pDisp	= NULL;
	hres = pUkn->QueryInterface(IID_IDispatch, (void **)&pDisp);
	pUkn->Release();
	if(FAILED(hres))
	{
		CoUninitialize();
		return;
	}

	DISPID		disp;
	BSTR		bstr = L"ErrTest";
	hres = pDisp->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_USER_DEFAULT, &disp);
	if(FAILED(hres))
	{
		pDisp->Release();
		CoUninitialize();
		return;
	}
	
	DISPPARAMS		param = {0};
	VARIANT			var;
	hres = pDisp->Invoke(disp, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, &var, NULL, NULL);
	if(FAILED(hres))
	{
		IErrorInfo   *pinfo;
		hres = ::GetErrorInfo(0, &pinfo);
		if(SUCCEEDED(hres))
		{
			BSTR		bstr;
			hres = pinfo->GetDescription(&bstr);
			//pinfo->GetHelpContext()
			if(SUCCEEDED(hres))
				MessageBoxW(m_hWnd, bstr, L"Error", MB_OK | MB_ICONERROR);
			::SysFreeString(bstr);
		}
	}

	pDisp->Release();
	CoUninitialize();
}
