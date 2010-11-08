// URL_ProtocolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "URL_Protocol.h"
#include "URL_ProtocolDlg.h"
#include ".\url_protocoldlg.h"

#include <atlbase.h>
#include <statreg.h>

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


// CURL_ProtocolDlg �Ի���



CURL_ProtocolDlg::CURL_ProtocolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CURL_ProtocolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CURL_ProtocolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CURL_ProtocolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CURL_ProtocolDlg ��Ϣ�������

BOOL CURL_ProtocolDlg::OnInitDialog()
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
	SetDlgItemText(IDC_EDIT1, GetCommandLine());
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CURL_ProtocolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CURL_ProtocolDlg::OnPaint() 
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
HCURSOR CURL_ProtocolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CURL_ProtocolDlg::OnBnClickedOk()
{
	// ע��
	CRegObject			reg;
	WCHAR				szModule[MAX_PATH];
	HRESULT				hres;
	CString				strMsg;

	memset(szModule, 0, sizeof(szModule));
	GetModuleFileNameW(AfxGetInstanceHandle(), szModule, sizeof(szModule));
	reg.AddReplacement(L"MODULE", szModule);
	hres = reg.ResourceRegister(szModule, IDR_URLPROTOCOL, L"REGISTRY");
	if(S_OK != hres)
	{
		strMsg.Format("ע��ʧ��: %08x", hres);
		AfxMessageBox(strMsg);
	}
	else
	{
		AfxMessageBox("ע��ɹ�!");
	}
}

void CURL_ProtocolDlg::OnBnClickedButton1()
{
	CRegObject			reg;
	WCHAR				szModule[MAX_PATH];
	HRESULT				hres;
	CString				strMsg;

	memset(szModule, 0, sizeof(szModule));
	GetModuleFileNameW(AfxGetInstanceHandle(), szModule, sizeof(szModule));
	reg.AddReplacement(L"MODULE", szModule);
	hres = reg.ResourceUnregister(szModule, IDR_URLPROTOCOL, L"REGISTRY");
	if(S_OK != hres)
	{
		strMsg.Format("����ʧ��: %08x", hres);
		AfxMessageBox(strMsg);
	}
	else
	{
		AfxMessageBox("���سɹ�!");
	}
}
