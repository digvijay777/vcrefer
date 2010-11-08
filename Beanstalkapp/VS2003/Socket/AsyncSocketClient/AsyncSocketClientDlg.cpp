// AsyncSocketClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AsyncSocketClient.h"
#include "AsyncSocketClientDlg.h"
#include ".\asyncsocketclientdlg.h"
#include <afxsock.h>
#include "MySocket.h"

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


// CAsyncSocketClientDlg �Ի���



CAsyncSocketClientDlg::CAsyncSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsyncSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAsyncSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LT_MSG, m_msg);
}

BEGIN_MESSAGE_MAP(CAsyncSocketClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BT_CONNECT, OnBnClickedBtConnect)
	ON_BN_CLICKED(IDC_BT_CLOSE, OnBnClickedBtClose)
	ON_BN_CLICKED(IDC_BT_SEND, OnBnClickedBtSend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAsyncSocketClientDlg ��Ϣ�������

BOOL CAsyncSocketClientDlg::OnInitDialog()
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
	GetDlgItem(IDC_BT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(FALSE);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CAsyncSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAsyncSocketClientDlg::OnPaint() 
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
HCURSOR CAsyncSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAsyncSocketClientDlg::OnBnClickedButton1()
{
	SOCKET			sock;
	SOCKADDR_IN		addr;
	int				nRet;

	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.204");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4045);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == sock)
	{
		AfxMessageBox("�����׽���ʧ��");
		return ;
	}
	do 
	{
		nRet = connect(sock, (SOCKADDR *)&addr, sizeof(SOCKADDR));
		if(SOCKET_ERROR == nRet)
		{
			AfxMessageBox("��������ʧ��");
			break;
		}
		char	buff[100]	= {0};
		recv(sock, buff, 100, 0);
		AfxMessageBox(buff);
		strcpy(buff, "quitaaaaa");
		send(sock, buff, strlen(buff) + 1, 0);
		recv(sock, buff, 100, 0);
		AfxMessageBox(buff);
	} while (0);
	closesocket(sock);
}

void CAsyncSocketClientDlg::OnBnClickedBtConnect()
{
	//��������
	if(NULL != g_pSocket)
		return;
	g_pSocket = new CMySocket(m_hWnd);
	g_pSocket->Create();
	g_pSocket->Connect("192.168.1.204", 4045);
	//::SendDlgItemMessage(m_hWnd, IDC_BT_CLOSE, WM_ENABLE, (WPARAM)TRUE, 0);
	//SendDlgItemMessage(IDC_BT_CLOSE, WM_ENABLE, 1, 0);
	//::SendDlgItemMessage(m_hWnd, IDC_BT_SEND, WM_ENABLE, 0, 0);
	//::SendDlgItemMessage(m_hWnd, IDC_BT_CONNECT, WM_ENABLE, 1, 0);
	//GetDlgItem(IDC_BT_CLOSE)->EnableWindow(TRUE);
}

void CAsyncSocketClientDlg::OnBnClickedBtClose()
{
	// �ر�����
	if(NULL == g_pSocket)
		return;
	g_pSocket->Close();
	delete g_pSocket;
	g_pSocket = NULL;
	GetDlgItem(IDC_BT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_CONNECT)->EnableWindow(TRUE);
}

void CAsyncSocketClientDlg::OnBnClickedBtSend()
{
	// ������Ϣ
	char	buff[4048]	= {0};
	int		length		= 0;

	GetDlgItemText(IDC_ET_SEND, buff, 4048);
	length = strlen(buff) + 1;
	if(1 == length)
	{
		AfxMessageBox("���ܷ��Ϳ��ַ���");
		return;
	}
	g_pSocket->Send(buff, length);
}

void CAsyncSocketClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(NULL != g_pSocket)
	{
		g_pSocket->Close();
		delete g_pSocket;
		g_pSocket = NULL;
	}
	CDialog::OnClose();
}
