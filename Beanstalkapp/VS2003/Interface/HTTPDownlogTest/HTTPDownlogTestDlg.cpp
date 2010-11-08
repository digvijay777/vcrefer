// HTTPDownlogTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HTTPDownlogTest.h"
#include "HTTPDownlogTestDlg.h"
#include ".\httpdownlogtestdlg.h"
#include "wininet.h"
#pragma comment(lib, "Wininet.lib")

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


// CHTTPDownlogTestDlg �Ի���



CHTTPDownlogTestDlg::CHTTPDownlogTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHTTPDownlogTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_signed = false;
}

void CHTTPDownlogTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BEGIN_MESSAGE_MAP(CHTTPDownlogTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CHTTPDownlogTestDlg ��Ϣ�������

BOOL CHTTPDownlogTestDlg::OnInitDialog()
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
//	SetDlgItemText(IDC_EDIT1, _T("http://www.baidu.com/index.php"));
//	SetDlgItemText(IDC_EDIT1, _T("http://www.jiedian.com.cn/Soft/UploadSoft/200904/2009040800544931.rar"));
//	SetDlgItemText(IDC_EDIT1, _T("http://dt.163.com/images/news/0605/news02053101_5.jpg"));
// 	SetDlgItemText(IDC_EDIT1, _T("http://download.microsoft.com/download/4/1/4/4140e2e0-0ad9-4438-ac52-da0e0429c0e6/w2ksp4_cn.exe"));
	SetDlgItemText(IDC_EDIT1, _T("http://sp.snswall.com/WindowsXP-KB935843-x86-CHS.exe"));
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CHTTPDownlogTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHTTPDownlogTestDlg::OnPaint() 
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
HCURSOR CHTTPDownlogTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHTTPDownlogTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CHTTPDownlogTestDlg::OnBnClickedButton1()
{
/*
		CHAR	szServerName[MAX_PATH]		= {"sp.snswall.com"};
		CHAR	szObjectName[MAX_PATH]		= {"/WindowsXP-KB935843-x86-CHS.exe"};
		DWORD	dwTime = 15000;
		CHAR* 	pAcceptTypes = "* / *";
		HINTERNET		m_hInterOpen				= NULL;
		HINTERNET		m_hInterConn				= NULL;
		HINTERNET		m_hHttpReq				= NULL;
	
		m_hInterOpen = InternetOpen( "Mozilla/4.0 (compatible)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
		InternetSetOption( m_hInterOpen, INTERNET_OPTION_CONNECT_TIMEOUT, (void*)&dwTime, 4 );
		m_hInterConn = InternetConnect( m_hInterOpen, szServerName, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, 
			INTERNET_SERVICE_HTTP, 0, 0 );
		m_hHttpReq = HttpOpenRequestA( m_hInterConn, "GET", szObjectName, "HTTP/1.1", NULL, 
			(LPCSTR *)&pAcceptTypes, INTERNET_FLAG_RELOAD | INTERNET_FLAG_RESYNCHRONIZE, 0);
	
		DWORD	dwReq = 0;
		DWORD	dwBufferLength = 4;
		if( NULL != m_hHttpReq )
		{
			if( HttpSendRequest(m_hHttpReq, NULL, 0, NULL, 0) )
			{
				if( !HttpQueryInfo(m_hHttpReq, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE,
					&dwReq, &dwBufferLength, NULL) || dwReq == HTTP_STATUS_OK	)
				{
					int i = 0;
					return ;
				}
			}
		}
	
	return;*/
	// TODO: Add your control notification handler code here
	if(false == m_signed)
	{
		m_signed = true;
		HANDLE handle = CreateThread(NULL, 0, DownThread, (LPVOID)this, 0, NULL);
		CloseHandle(handle);
	}
	else
	{
		AfxMessageBox(_T("���ؽ�����������"));
	}
}

void CHTTPDownlogTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	TCHAR			szURL[MAX_PATH]			= {0};
	
	GetDlgItemText(IDC_EDIT1, szURL, MAX_PATH);

	DWORD byteread=0;
	char buffer[100];
	memset(buffer,0,100);
	HINTERNET internetopen;

	internetopen=InternetOpen("Testing",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if (internetopen==NULL)
	{ 
		//cout<<"Internet open failed!"<<endl;
		MessageBox(_T("Internet open failed!"));
		return;
	}

	HINTERNET internetopenurl;
	internetopenurl=InternetOpenUrl(internetopen,szURL,NULL,0,INTERNET_FLAG_RELOAD,0);   
	if (internetopenurl==NULL)
	{ 
		//cout<<"Internet open url failed!"<<endl; 
		MessageBox(_T("Internet open url failed!"));
		goto there;
	}

	BOOL hwrite;
	DWORD written;
	HANDLE createfile;
	createfile=CreateFile("c:\\sss.jpg",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if (createfile==INVALID_HANDLE_VALUE)
	{  
		//cout<<"Create File failed!"<<endl;
		MessageBox(_T("Create File failed!"));
		goto next;
	}

	BOOL internetreadfile;
	while(1)
	{
		internetreadfile=InternetReadFile(internetopenurl,buffer,sizeof(buffer),&byteread);
		if(byteread==0)  
			break;
		hwrite=WriteFile(createfile,buffer,sizeof(buffer),&written,NULL);
		if (hwrite==0)
		{
			//cout<<"Write to file failed!"<<endl;
			MessageBox(_T("Write to file failed!"));
			goto here;
		}
	}

	//cout<<"Finished downloading!"<<endl;
	MessageBox(_T("Finished downloading!"));

here:
	CloseHandle(createfile);
next: 
	InternetCloseHandle(internetopenurl);
there:
	InternetCloseHandle(internetopen);

	//��������CSDN���ͣ�ת�������������http://blog.csdn.net/hityct1/archive/2009/01/25/3852759.aspx
}

DWORD WINAPI CHTTPDownlogTestDlg::DownThread(LPVOID pParameter)
{
	CHTTPDownlogTestDlg*	pThis		= (CHTTPDownlogTestDlg *)pParameter;
	HINTERNET		session				= NULL;
	HINTERNET		connect				= NULL;
	HINTERNET		http				= NULL;
	TCHAR			url[MAX_PATH]		= {0};
	TCHAR			server[MAX_PATH]	= {0};
	TCHAR			file[MAX_PATH]		= {0};
	TCHAR			name[MAX_PATH]		= _T("C:\\MyDownLog\\");
	BYTE			lpReadeBuffer[1024 * 8]	= {0};
	TCHAR			header[8]			= {0};
	TCHAR			lpLength[20]		= _T("�ļ���С:");
	DWORD			dwTimeout			= 15000;
	DWORD			dwReq				= 0;
	DWORD			dwBufferLength		= 500;
	DWORD			dwIndex				= 0;
	CString			str;

	pThis->GetDlgItemText(IDC_EDIT1, url, MAX_PATH);
	if(_tcslen(url) < 7)
		goto theEnd;
	_tcsncpy(header, url, 7);
	if(_tcsicmp(header, _T("http://")) != 0)
		goto theEnd;
	_tcscpy(server, url + 7);
	TCHAR *p = _tcschr(server, '/');
	if(NULL != p)
	{
		*p = '\0';
		_tcscat(name, _tcsrchr(url, '/') + 1);
	}
	_tcscpy(file, url + _tcslen(server) + 8);

	session = InternetOpen(_T("Mozilla/4.0 (compatible)"), INTERNET_OPEN_TYPE_DIRECT , NULL, NULL, 0);
	if(NULL == session)
	{
		AfxMessageBox(_T("�����Ựʧ��"));
		goto theEnd;
	}
	connect = InternetConnect(session, server, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(NULL == connect)
	{
		AfxMessageBox(_T("��������ʧ��"));
		goto theSession;
	}
// 	CHAR* 	pAcceptTypes = "*/*";
	http = HttpOpenRequest(connect, "GET", file, _T("HTTP/1.1"), NULL,/* (LPCSTR *)&pAcceptTypes*/NULL, INTERNET_FLAG_RELOAD, 0);
	if(NULL == http)
	{
		AfxMessageBox(_T("����Ựʧ��"));
		goto theConnect;
	}
	if( HttpSendRequest(http, NULL, 0, NULL, 0) )
	{
		//dwBufferLength = 4;
		if( !HttpQueryInfo(http, HTTP_QUERY_CONTENT_LENGTH,// | HTTP_QUERY_FLAG_NUMBER,
			&lpReadeBuffer, &dwBufferLength, NULL))   //dwBufferLengthһ��Ҫ�г�ʼ��С
		{
			AfxMessageBox(_T("��ȡ�ļ���Сʧ��"));
			goto theHttp;
		}
		pThis->m_progress.SetRange(0, 100);
		DWORD		allLength = atol((char*)lpReadeBuffer);
		memset(header, 0, sizeof(TCHAR) * 8);
		_itot(allLength, header, 10);
		_tcscat(lpLength, header);
		_tcscat(lpLength, "Bit");
		pThis->SetDlgItemText(IDC_STATIC_L, lpLength);

		FILE *ffile = fopen(name, "wb+");
		if(NULL == ffile)
		{
			AfxMessageBox(_T("�����ļ�ʧ��"));
			goto theHttp;
		}
		do{
			InternetReadFile(http, lpReadeBuffer, 1024 * 8, &dwBufferLength);
			dwReq += dwBufferLength;
			if(dwBufferLength > 0)
				fwrite(lpReadeBuffer, 1, dwBufferLength, ffile);
			int i = (int)((float)dwReq / (float)allLength * (float)100);
			pThis->m_progress.SetPos(i);
			//Sleep(100);
		}while(dwBufferLength > 0);
		fclose(ffile);

	}

	str.Format(_T("�������:%s"), name);
	AfxMessageBox(str);


theHttp:
	InternetCloseHandle(http);
theConnect:
	InternetCloseHandle(connect);
theSession:
	InternetCloseHandle(session);
theEnd:
	pThis->m_signed = false;
	return 0;
}