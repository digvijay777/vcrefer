// NetClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetClient.h"
#include "NetClientDlg.h"
#include ".\netclientdlg.h"

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


// CNetClientDlg �Ի���



CNetClientDlg::CNetClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sockChart = NULL;
	GetModuleFileName(NULL, m_strINI.GetBufferSetLength(1024), 1024);
	m_strINI.ReleaseBuffer();
	m_strINI = m_strINI.Left(m_strINI.ReverseFind('.'));
	m_strINI += ".ini";
}

void CNetClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CNetClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
// 	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BT_CONNECT, OnBnClickedBtConnect)
	ON_BN_CLICKED(IDC_BT_SEND, OnBnClickedBtSend)
	ON_BN_CLICKED(IDC_BT_SENDNAT, OnBnClickedBtSendnat)
END_MESSAGE_MAP()


// CNetClientDlg ��Ϣ�������

BOOL CNetClientDlg::OnInitDialog()
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
	HANDLE		hTread		= 0;

	if(1 == GetPrivateProfileInt("Nat", "TCP", 1, m_strINI.GetBuffer()))
	{
		hTread = CreateThread(NULL, 0, ListenSocketThread, this, 0, NULL);
	}
	else
	{
		hTread = CreateThread(NULL, 0, RecvFromSocketThread, this, 0, NULL);
	}
	CloseHandle(hTread);

	EnableChartWin();

	SetDlgItemText(IDC_ET_SENDPORT, "0");
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CNetClientDlg::EnableChartWin(BOOL bConnect /* = FALSE */)
{
	GetDlgItem(IDC_ET_IP)->EnableWindow(FALSE == bConnect);
	GetDlgItem(IDC_ET_PORT)->EnableWindow(FALSE == bConnect);
	GetDlgItem(IDC_ET_SENDPORT)->EnableWindow(FALSE == bConnect);
	SetDlgItemText(IDC_BT_CONNECT, bConnect?"�Ͽ�":"����");
	GetDlgItem(IDC_ET_MSG)->EnableWindow(FALSE != bConnect);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(FALSE != bConnect);
}

void CNetClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNetClientDlg::OnPaint() 
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
HCURSOR CNetClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNetClientDlg::OnBnClickedButton1()
{
	SOCKET				sock		= NULL;
	SOCKADDR_IN			addr		= {0};
	int					flag		= 1;
	int					len			= sizeof(flag);
	int					nRev		= 0;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(INVALID_SOCKET  != sock);

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

	addr.sin_port = htons(4040);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	VERIFY(0 == bind(sock, (SOCKADDR*)&addr, sizeof(addr)));

	addr.sin_port = htons(4040);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.4");
	nRev = connect(sock, (SOCKADDR *)&addr, sizeof(addr));
	if(0 == nRev)
	{
		CHAR		szData[50]	= {" this is port 4040"};
		
		len = strlen(szData) + 1;
		szData[0] = 0x1;
		send(sock, szData, len, 0);
		recv(sock, szData, strlen(szData)-1, 0);
	}

	shutdown(sock, SD_BOTH);
	// �ر�
	flag = 0;
	len = sizeof(flag);
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
	closesocket(sock);
}

DWORD CNetClientDlg::ListenSocketThread(LPVOID lpParam)
{
	CNetClientDlg*		pDlg		= (CNetClientDlg *)lpParam;
	SOCKET				sock		= NULL;
	SOCKADDR_IN			addr		= {0};
	int					flag		= 1;
	int					len			= sizeof(flag);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(INVALID_SOCKET  != sock);

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

	addr.sin_port = htons(GetPrivateProfileInt("Nat", "Port", 6666, pDlg->m_strINI.GetBuffer()));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	VERIFY(0 == bind(sock, (SOCKADDR*)&addr, sizeof(addr)));
	listen(sock, 5);

	pDlg->ListenSocket(sock);
	
	shutdown(sock, SD_BOTH);
	flag = 0;
	len = sizeof(flag);
//	setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, (char *)&flag, len);
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
	closesocket(sock);
	
	return 0;
}

void CNetClientDlg::ListenSocket(SOCKET sock)
{
	SOCKADDR_IN			addr		= {0};
	int					nLength		= sizeof(addr);
	SOCKET				client		= NULL;
	CString				strLine;
	CHAR				szBuff[1024];
	int					flag		= 1;
	int					len			= sizeof(flag);

	m_list.AddString("���������ʼ��");
	do
	{
		nLength = sizeof(addr);
		memset(&addr, 0, sizeof(client));

		client = accept(sock, (SOCKADDR *)&addr, &nLength);

		if(INVALID_SOCKET == client)
			break;

		// �˿�����
		flag = 1;
		len = sizeof(flag);
		setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

		memset(szBuff, 0, sizeof(szBuff));
		recv(client, szBuff, sizeof(szBuff) - 1, 0);

		strLine.Format("%s:%d say %s", inet_ntoa(addr.sin_addr), (unsigned int)ntohs(addr.sin_port), &szBuff[1]);
		m_list.AddString(strLine);
		send(client, "ok!", 4, 0);
		// �ر�

		setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);
		shutdown(client, SD_BOTH);
		setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

		flag = 0;
		len = sizeof(flag);
	//	setsockopt(m_sockChart, SOL_SOCKET, SO_DONTROUTE, (char *)&flag, len);
		setsockopt(client, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
		closesocket(client);

		Sleep(1500);
		if('FFFF' == *(DWORD *)&szBuff[0])
		{
			SetDlgItemText(IDC_ET_IP, inet_ntoa(addr.sin_addr));
			SetDlgItemText(IDC_ET_PORT, itoa((unsigned int)ntohs(addr.sin_port), szBuff, 10));
			CString			strSend;
			GetPrivateProfileString("Nat", "SendPort", "6666", strSend.GetBufferSetLength(1024), 1024, m_strINI.GetBuffer());
			strSend.ReleaseBuffer();
			SetDlgItemText(IDC_ET_SENDPORT, strSend.GetBuffer());

// 			shutdown(sock, SD_BOTH);
// 			flag = 0;
// 			len = sizeof(flag);
// 			//	setsockopt(m_sockChart, SOL_SOCKET, SO_DONTROUTE, (char *)&flag, len);
// 			setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
// 			closesocket(sock);

// 			Sleep(1500);
			PostMessage(WM_COMMAND, IDC_BT_CONNECT);
			break;
		}
	}while(TRUE);

	m_list.AddString("�������ֹͣ");
}

void CNetClientDlg::OnBnClickedBtConnect()
{
	CString				strIP;
	CString				strCmd;
	int					nPort;
	int					nSendPort;
	int					flag		= 1;
	int					len			= sizeof(flag);

	GetDlgItemText(IDC_BT_CONNECT, strCmd);
	nSendPort = GetDlgItemInt(IDC_ET_SENDPORT);
	if(strCmd == "�Ͽ�")
	{
		shutdown(m_sockChart, SD_BOTH);
		// �ر�
		flag = 0;
		len = sizeof(flag);
		setsockopt(m_sockChart, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
		closesocket(m_sockChart);
		EnableChartWin(FALSE);
		m_sockChart = NULL;
		SetDlgItemText(IDC_BT_CONNECT, "����");
		return;
	}

	// ����
	GetDlgItemText(IDC_ET_IP, strIP);
	nPort = GetDlgItemInt(IDC_ET_PORT);

	SOCKADDR_IN			addr		= {0};
	int					nRev		= 0;

	m_sockChart = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(0 != nSendPort)
	{
		flag = 1;
		len = sizeof(flag);
		setsockopt(m_sockChart, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

		addr.sin_port = htons(GetPrivateProfileInt("Nat", "Port", 6666, m_strINI.GetBuffer()));
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = ADDR_ANY;
		VERIFY(0 == bind(m_sockChart, (SOCKADDR*)&addr, sizeof(addr)));
	}

	addr.sin_port = htons(nPort);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(strIP.GetBuffer());
	nRev = connect(m_sockChart, (SOCKADDR *)&addr, sizeof(addr));
	if(0 == nRev)
	{
		EnableChartWin(TRUE);
		SetDlgItemText(IDC_BT_CONNECT, "�Ͽ�");
	}
	else
	{
		CString		strErr;
	
		strErr.Format("����%sʧ��: %d", strIP.GetBuffer(), WSAGetLastError());
		// �Ͽ�
		flag = 0;
		len = sizeof(flag);
		setsockopt(m_sockChart, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
		closesocket(m_sockChart);
		EnableChartWin(FALSE);
		m_sockChart = NULL;
		MessageBox(strErr, "", MB_OK|MB_ICONERROR);
	}
}

void CNetClientDlg::OnBnClickedBtSend()
{
	if(NULL == m_sockChart)
		return ;

	CString		strMsg;
	CString		strRcv;

	GetDlgItemText(IDC_ET_MSG, strMsg);

	send(m_sockChart, strMsg.GetBuffer(), strMsg.GetLength()+1, 0);
	recv(m_sockChart, strRcv.GetBufferSetLength(1024), 1024, 0);
	strMsg.Format("Send %s", strRcv.GetBuffer());
	m_list.AddString(strMsg);

	OnBnClickedBtConnect();
}

void CNetClientDlg::OnBnClickedBtSendnat()
{
	CString				strIP;
	CString				strCmd;
	SOCKET				sock;
	int					nPort;
	int					nSendPort;
	int					flag		= 1;
	int					len			= sizeof(flag);

	// ����
	GetDlgItemText(IDC_ET_IP, strIP);
	nPort = GetDlgItemInt(IDC_ET_PORT);

	SOCKADDR_IN			addr		= {0};
	int					nRev		= 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(0 != nSendPort)
	{
		flag = 1;
		len = sizeof(flag);
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

		addr.sin_port = htons(GetPrivateProfileInt("Nat", "Port", 6666, m_strINI.GetBuffer()));
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = ADDR_ANY;
		VERIFY(0 == bind(sock, (SOCKADDR*)&addr, sizeof(addr)));
	}

	addr.sin_port = htons(nPort);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(strIP.GetBuffer());

	sendto(sock, "FFFF", 5, 0, (SOCKADDR *)&addr, sizeof(addr));
	closesocket(sock);
}

DWORD CNetClientDlg::RecvFromSocketThread(LPVOID lpParam)
{
	CNetClientDlg*		pDlg		= (CNetClientDlg *)lpParam;
	SOCKET				sock		= NULL;
	SOCKADDR_IN			addr		= {0};
	int					flag		= 1;
	int					len			= sizeof(flag);

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	ASSERT(INVALID_SOCKET  != sock);

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, len);

	addr.sin_port = htons(GetPrivateProfileInt("Nat", "Port", 6666, pDlg->m_strINI.GetBuffer()));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	VERIFY(0 == bind(sock, (SOCKADDR*)&addr, sizeof(addr)));
	
	CHAR		szBuff[1024];
	while(true)
	{
		memset(szBuff, 0, sizeof(szBuff));
		SOCKADDR_IN			addrClient		= {0};
		int					nLen			= sizeof(addrClient);

		recvfrom(sock, szBuff, sizeof(szBuff), 0, (SOCKADDR *)&addrClient, &nLen);

		CString		strLine;
		strLine.Format("%s:%d say %s", inet_ntoa(addrClient.sin_addr), (unsigned int)ntohs(addrClient.sin_port), &szBuff[1]);
		pDlg->m_list.AddString(strLine);

		if('FFFF' == *(DWORD *)&szBuff[0])
		{
			pDlg->SetDlgItemText(IDC_ET_IP, inet_ntoa(addrClient.sin_addr));
			pDlg->SetDlgItemText(IDC_ET_PORT, itoa((unsigned int)ntohs(addrClient.sin_port), szBuff, 10));
			CString			strSend;
			GetPrivateProfileString("Nat", "SendPort", "6666", strSend.GetBufferSetLength(1024), 1024, pDlg->m_strINI.GetBuffer());
			strSend.ReleaseBuffer();
			pDlg->SetDlgItemText(IDC_ET_SENDPORT, strSend.GetBuffer());

			// 			shutdown(sock, SD_BOTH);
			// 			flag = 0;
			// 			len = sizeof(flag);
			// 			//	setsockopt(m_sockChart, SOL_SOCKET, SO_DONTROUTE, (char *)&flag, len);
			// 			setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
			// 			closesocket(sock);

			// 			Sleep(1500);
			pDlg->PostMessage(WM_COMMAND, IDC_BT_CONNECT);
		}
	}

	shutdown(sock, SD_BOTH);
	flag = 0;
	len = sizeof(flag);
	//	setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, (char *)&flag, len);
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&flag, len);
	closesocket(sock);

	return 0;
}
