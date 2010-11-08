// DDEClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DDEClient.h"
#include "DDEClientDlg.h"
#include ".\ddeclientdlg.h"

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


// CDDEClientDlg �Ի���



CDDEClientDlg::CDDEClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDDEClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDDEClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDDEClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDDEClientDlg ��Ϣ�������

BOOL CDDEClientDlg::OnInitDialog()
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

void CDDEClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDDEClientDlg::OnPaint() 
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
HCURSOR CDDEClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDDEClientDlg::OnBnClickedButton1()
{
/*
	CHAR				szHello[]	= "Hello World!";
	DWORD				dwSize		= sizeof(COPYDATASTRUCT) + strlen(szHello) + 1;
	HGLOBAL				hData		= GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT | GMEM_SHARE, dwSize);
	COPYDATASTRUCT*		pData		= (COPYDATASTRUCT *)GlobalLock(hData);

	pData->cbData = dwSize;
	pData->dwData = 5;
	pData->lpData = pData + 1;
	strcpy((CHAR *)pData->lpData, szHello);
	
	::SendMessage(HWND_BROADCAST, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)pData);
	GlobalUnlock(hData);
	GlobalFree(hData);*/
	char				szHello[1024]	= "Hello World!";
	DWORD				dwSize			= sizeof(szHello);
	HGLOBAL				hData			= GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, dwSize);
	HGLOBAL				hCopy			= GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(COPYDATASTRUCT));
	COPYDATASTRUCT		*pCopy			= (COPYDATASTRUCT *)GlobalLock(hCopy);
	CHAR*				pData			= (CHAR *)GlobalLock(hData);

	strcpy(pData, szHello);
	GlobalUnlock(hData);
	pCopy->cbData = sizeof(COPYDATASTRUCT);
	pCopy->dwData = 1024;
	pCopy->lpData = hData;
	GlobalUnlock(hCopy);

	::SendMessage(::FindWindow(NULL, "DDEServer"), WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)hCopy);
	
	pData = (CHAR*)GlobalLock(hData);
	strcpy(szHello, pData);
	GlobalUnlock(hData);
	GlobalFree(hData);
	GlobalFree(hCopy);
}
