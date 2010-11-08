// ProxySocksDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProxySocks.h"
#include "ProxySocksDlg.h"
#include ".\proxysocksdlg.h"
#include <afxsock.h>
#include "../../../ExtClass/Net/XSocksProxy.h"
#include "../../../ExtClass/XNet.h"

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


// CProxySocksDlg �Ի���



CProxySocksDlg::CProxySocksDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProxySocksDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProxySocksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProxySocksDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CProxySocksDlg ��Ϣ�������

BOOL CProxySocksDlg::OnInitDialog()
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

void CProxySocksDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProxySocksDlg::OnPaint() 
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
HCURSOR CProxySocksDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProxySocksDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CXSocksProxy		proxy;
	CHAR				szBuff[1024];

	if(FALSE == proxy.Connect("192.168.1.6", 1080, "xxj", "xxx"))
		return;

// 	if(FALSE == proxy.ConnectSocks5(NULL, NULL, "161.250.41.200", 8603))
// 		return;
// 	if(FALSE == proxy.ProxySocks5("161.250.41.200", 8603))
// 		return;
	if(FALSE == proxy.ProxySocks5("161.250.41.200", 8603))
		return ;

	memset(szBuff, 0, 1024);
	recv(proxy.m_hSocket, szBuff, 1024, 0);
	AfxMessageBox(szBuff);
}

void CProxySocksDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSocket		sock;
	CHAR		szBuff[1024];

	if(FALSE == sock.Create())
		return;

	if(FALSE == sock.Connect("161.250.41.200", 8603))
		return;

	memset(szBuff, 0, 1024);
	if(FALSE == sock.Receive(szBuff, 1024))
		return;
	
	AfxMessageBox(szBuff);
}

void CProxySocksDlg::OnBnClickedButton3()
{
	SOCKET			sock				= INVALID_SOCKET;
	LPCSTR			lpGet				= "GET /\r\n";
	CHAR			szBuffer[1024]		= {0};

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(FALSE == XNet::ConnectWidthProxy4(sock, "192.168.1.16", 80, "yj.net", 1080))
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(FALSE == XNet::ConnectWidthProxy5(sock, "192.168.1.16", 80, "yj.net", 1080, "yj", "1234"))
		{
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(FALSE == XNet::ConnectWidthProxy5(sock, "192.168.1.16", 80, "yj.net", 1080, "yj", "123"))
				return;
		}
	}
	
	send(sock, lpGet, strlen(lpGet) + 1, 0);
	recv(sock, szBuffer, sizeof(szBuffer), 0);

}
