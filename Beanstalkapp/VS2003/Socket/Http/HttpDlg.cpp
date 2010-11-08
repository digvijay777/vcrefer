// HttpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Http.h"
#include "HttpDlg.h"
#include ".\httpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <afxinet.h>

#pragma comment(lib, "Wininet.lib");
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


// CHttpDlg �Ի���



CHttpDlg::CHttpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHttpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHttpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHttpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CHttpDlg ��Ϣ�������

BOOL CHttpDlg::OnInitDialog()
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

void CHttpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHttpDlg::OnPaint() 
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
HCURSOR CHttpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHttpDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
// 	OnOK();
}

void CHttpDlg::OnBnClickedButton1()
{
	// POST	���ݲ���
	HINTERNET			hInterConnect		= NULL;
	HINTERNET			hInterURL			= NULL;
	HINTERNET			hInterSession		= NULL;
	CString				szServer, szFilePath;
	INTERNET_PORT		nPort				= 80;
	DWORD				dwType				= AFX_INET_SERVICE_HTTP;
	CString				szData;
	LRESULT				lRet				= 0;
	CString				szValidateURL;

	GetDlgItemText(IDC_ET_URL, szValidateURL);
	GetDlgItemText(IDC_ET_MSG, szData);

	AfxParseURL(szValidateURL.GetBuffer(), dwType, szServer, szFilePath, nPort);
	do 
	{
		hInterConnect = InternetOpen("ClientMgr.ocx", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if(NULL == hInterConnect)
			break;
 		hInterURL = InternetConnect(hInterConnect, szServer, nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if(NULL == hInterURL)
			break;
		hInterSession = HttpOpenRequest(hInterURL, "POST", szFilePath, "HTTP/1.1", NULL, NULL,  INTERNET_FLAG_RELOAD, 0);
		if(NULL == hInterSession)
			break;
// 		szData.Format("user=%s&password=%s&autologin=%d", (LPCSTR)wParam, (LPCSTR)lParam, m_bValidateAuto);
		CString  szHead;
		szHead = "Accept-Language: zh-cn \r\nContent-Type: application/x-www-form-urlencoded\r\n";
		if(!HttpSendRequest(hInterSession, szHead.GetBuffer(), szHead.GetLength(), szData.GetBuffer(), szData.GetLength()))
			break;
		BYTE		lpReadBuffer[5120]	= {0};
		DWORD		dwBufferLength		= 1024;
		//��ȡ�ļ���С
		if( !HttpQueryInfo(hInterSession, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			&lpReadBuffer, &dwBufferLength, NULL))   //dwBufferLengthһ��Ҫ�г�ʼ��С
			break;
		if(4 != dwBufferLength)
			break;
		dwBufferLength = *((LONG *)&lpReadBuffer[0]);
		if(dwBufferLength > sizeof(lpReadBuffer))	//̫���˲������
			break;
		if(!InternetReadFile(hInterSession, lpReadBuffer, dwBufferLength, &dwBufferLength))
			break;
		CString szValidate((char *)lpReadBuffer);
		SetDlgItemText(IDC_ET_MSG, szValidate);
	} while (0);

	if(NULL != hInterSession)
		InternetCloseHandle(hInterSession);
	if(NULL != hInterURL)
		InternetCloseHandle(hInterURL);
	if(NULL != hInterConnect)
		InternetCloseHandle(hInterConnect);
}

void CHttpDlg::OnBnClickedButton3()
{
	// Cookie ����
	CHAR			szData[1024];
	DWORD			dwSize;
	BOOL			bRes;

	memset(szData, 0, sizeof(szData));
	dwSize = sizeof(szData);
	bRes = InternetGetCookie("http://192.168.1.6/yj/session.asp", NULL, szData, &dwSize);
	int i = 0;
}

void CHttpDlg::OnBnClickedButton2()
{
	// Get	���ݲ���
	HINTERNET			hInterConnect		= NULL;
	HINTERNET			hInterURL			= NULL;
	HINTERNET			hInterSession		= NULL;
	CString				szServer, szFilePath;
	INTERNET_PORT		nPort				= 80;
	DWORD				dwType				= AFX_INET_SERVICE_HTTP;
	CString				szData;
	LRESULT				lRet				= 0;
	CString				szValidateURL;

	GetDlgItemText(IDC_ET_URL, szValidateURL);
	GetDlgItemText(IDC_ET_MSG, szData);

	AfxParseURL(szValidateURL.GetBuffer(), dwType, szServer, szFilePath, nPort);
	do 
	{
		hInterConnect = InternetOpen("ClientMgr.ocx", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if(NULL == hInterConnect)
			break;
		// 		hInterURL = InternetConnect(hInterConnect, szServer, nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		hInterURL = InternetConnect(hInterConnect, szServer, nPort, "small", "123", INTERNET_SERVICE_HTTP, 0, 0);
		if(NULL == hInterURL)
			break;
		hInterSession = HttpOpenRequest(hInterURL, "GET", szFilePath, "HTTP/1.1", NULL, NULL,  INTERNET_FLAG_RELOAD, 0);
		if(NULL == hInterSession)
			break;
		// 		szData.Format("user=%s&password=%s&autologin=%d", (LPCSTR)wParam, (LPCSTR)lParam, m_bValidateAuto);
		CString  szHead;
		szHead = "Accept-Language: zh-cn \r\n";
		if(!HttpSendRequest(hInterSession, szHead.GetBuffer(), szHead.GetLength(), szData.GetBuffer(), szData.GetLength()))
			break;
		BYTE		lpReadBuffer[5120]	= {0};
		DWORD		dwBufferLength		= 1024;
		//��ȡ�ļ���С
		if( !HttpQueryInfo(hInterSession, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			&lpReadBuffer, &dwBufferLength, NULL))   //dwBufferLengthһ��Ҫ�г�ʼ��С
			break;
		if(4 != dwBufferLength)
			break;
		dwBufferLength = *((LONG *)&lpReadBuffer[0]);
		if(dwBufferLength > sizeof(lpReadBuffer))	//̫���˲������
			break;
		if(!InternetReadFile(hInterSession, lpReadBuffer, dwBufferLength, &dwBufferLength))
			break;
		CString szValidate((char *)lpReadBuffer);
		SetDlgItemText(IDC_ET_MSG, szValidate);
	} while (0);

	if(NULL != hInterSession)
		InternetCloseHandle(hInterSession);
	if(NULL != hInterURL)
		InternetCloseHandle(hInterURL);
	if(NULL != hInterConnect)
		InternetCloseHandle(hInterConnect);
}
