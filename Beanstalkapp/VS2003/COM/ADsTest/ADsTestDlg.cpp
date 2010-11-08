// ADsTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADsTest.h"
#include "ADsTestDlg.h"
#include ".\adstestdlg.h"
#include <Iads.h>
#include <Adshlp.h>

#import "f:\iisext.dll" no_namespace

#pragma comment(lib, "ActiveDS.lib")
#pragma comment(lib, "Adsiid.lib")

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


// CADsTestDlg �Ի���



CADsTestDlg::CADsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADsTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CADsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CADsTestDlg ��Ϣ�������

BOOL CADsTestDlg::OnInitDialog()
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

void CADsTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CADsTestDlg::OnPaint() 
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
HCURSOR CADsTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CADsTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
// 	OnOK();
}

void CADsTestDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CADsTestDlg::OnBnClickedButton2()
{
	::CoInitialize(NULL);
/*
	IADsUser *pUser;
	HRESULT hr = ADsGetObject(L"WinNT://./test",IID_IADsUser,(void**)&pUser);
	if (FAILED(hr)) {return ;}
	BSTR bstr;
	VARIANT_BOOL	var;
	hr = pUser->get_PasswordRequired(&var);
*/
	//ADsGetObject()

/*
		IADs	*pADSI;
		HRESULT		hr = ADsGetObject(L"IIS://localhost/W3SVC", IID_IADs, (void **)&pADSI);
		if(FAILED(hr))
		{
			AfxMessageBox("�õ��ӿ�ʧ��");
		}
		CComBSTR	bstr = "EnableWebServiceExtension";
		CComVariant var = "ASP";
		DISPID		disp;
		DISPPARAMS  param = {0};
	
		param.cArgs = 1;
		param.rgvarg = new VARIANT[param.cArgs];
		param.rgvarg[0].bstrVal = ::SysAllocString(L"ASP"); 
		param.cNamedArgs = 0;
		param.
	
		hr = pADSI->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_SYSTEM_DEFAULT, &disp);
		hr = pADSI->Invoke(disp, IID_NULL, 0, DISPATCH_METHOD, &param, NULL, NULL, NULL);
		hr = pADSI->Put(bstr, var);
		hr = pADSI->SetInfo();
*/
	IISWebService *pIIS;
	HRESULT hr = ADsGetObject(L"IIS://localhost/W3SVC", __uuidof(IISWebService), (void **)&pIIS);
	pIIS->EnableWebServiceExtension(L"ASP");
	pIIS->SetInfo();
	//printf("User: %S\n", bstr);
//	TRACE("User: %S\n", bstr);
//	SysFreeString(bstr);


}
