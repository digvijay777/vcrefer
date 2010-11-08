// SocketClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"
#include ".\socketclientdlg.h"
#include <Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UM_RECV			(WM_USER+1)

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


// CSocketClientDlg �Ի���



CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_MESSAGE(UM_RECV, OnUmRecv)
END_MESSAGE_MAP()


// CSocketClientDlg ��Ϣ�������

BOOL CSocketClientDlg::OnInitDialog()
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
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		MessageBox("��ʼ���׽���ʧ��");
		return TRUE;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		MessageBox("��ʼ���׽���ʧ��");
		return TRUE;
	}
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketClientDlg::OnPaint() 
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
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSocketClientDlg::OnBnClickedButton1()
{
	//ͬ�����ӷ���������
	CString			str;
	SOCKET			server		= {0};
	SOCKADDR_IN		addr		= {0};
	int				nRet		= 0;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET != server)
	{
		do 
		{
			addr.sin_port = htons(8603);
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.204");
			//����
			nRet = connect(server, (SOCKADDR*)&addr, sizeof(SOCKADDR));
			if(0 != nRet)
			{
				str.Format("connect error:%d\n", WSAGetLastError());
				break;
			}
			//��������
			char recvbuf[100];
			memset(recvbuf, 0, 100);
			nRet = recv(server, recvbuf, 100, 0);
			if(0 != nRet)
			{
				str.Format("recv error:%d\n", WSAGetLastError());
			}
			str.Format("Server say:%s\n", recvbuf);
		} while (0);
		closesocket(server);
	}
	else
	{
		str.Format("socket error:%d", WSAGetLastError());
	}

	MessageBox(str);
}

void CSocketClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	WSACleanup();
	CDialog::OnClose();
}

void CSocketClientDlg::OnBnClickedButton2()
{
	//�첽���ӷ���������
	CString			str;
	SOCKET			server		= {0};
	SOCKADDR_IN		addr		= {0};
	int				nRet		= 0;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET != server)
	{
		do 
		{
			addr.sin_port = htons(4045);
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.204");
			//ע����Ϣ
			nRet = WSAAsyncSelect(server, m_hWnd, UM_RECV, FD_READ | FD_CLOSE | FD_CONNECT);
			if(SOCKET_ERROR == nRet)
			{
				str.Format("WSAAsyncSelect error:%d", WSAGetLastError());
			}
			//����
			nRet = connect(server, (SOCKADDR*)&addr, sizeof(SOCKADDR));
		} while (0);
		//closesocket(server);
	}
	else
	{
		str.Format("socket error:%d", WSAGetLastError());
	}

	if(str.GetLength() > 0)
		MessageBox(str);
}

LRESULT CSocketClientDlg::OnUmRecv(WPARAM wParam, LPARAM lParam)
{
	if(WSAGETSELECTERROR(lParam))
	{
		closesocket(wParam);
		return 0;
	}

	char  szBuffer[100] = {0};
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		recv((SOCKET)wParam,  szBuffer, 100, 0);
		AfxMessageBox(szBuffer);
		break;
	case FD_CLOSE:
		closesocket((SOCKET)wParam);
		break;
	case FD_CONNECT:

		break;
	}
	return 0;
}
