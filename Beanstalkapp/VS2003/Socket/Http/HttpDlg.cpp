// HttpDlg.cpp : 实现文件
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


// CHttpDlg 对话框



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


// CHttpDlg 消息处理程序

BOOL CHttpDlg::OnInitDialog()
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
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHttpDlg::OnPaint() 
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
	// POST	数据测试
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
		//获取文件大小
		if( !HttpQueryInfo(hInterSession, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			&lpReadBuffer, &dwBufferLength, NULL))   //dwBufferLength一定要有初始大小
			break;
		if(4 != dwBufferLength)
			break;
		dwBufferLength = *((LONG *)&lpReadBuffer[0]);
		if(dwBufferLength > sizeof(lpReadBuffer))	//太大了不会接收
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
	// Cookie 测试
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
	// Get	数据测试
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
		//获取文件大小
		if( !HttpQueryInfo(hInterSession, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			&lpReadBuffer, &dwBufferLength, NULL))   //dwBufferLength一定要有初始大小
			break;
		if(4 != dwBufferLength)
			break;
		dwBufferLength = *((LONG *)&lpReadBuffer[0]);
		if(dwBufferLength > sizeof(lpReadBuffer))	//太大了不会接收
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
