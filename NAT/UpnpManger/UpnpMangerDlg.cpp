// UpnpMangerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpnpManger.h"
#include "UpnpMangerDlg.h"
#include ".\upnpmangerdlg.h"
#include "AddUpnpDlg.h"

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


// CUpnpMangerDlg �Ի���



CUpnpMangerDlg::CUpnpMangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpnpMangerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpnpMangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_ET_IP, m_ip);
}

BEGIN_MESSAGE_MAP(CUpnpMangerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_REFURISH, OnBnClickedBtRefurish)
	ON_BN_CLICKED(IDC_BT_ADD, OnBnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_DELETE, OnBnClickedBtDelete)
END_MESSAGE_MAP()


// CUpnpMangerDlg ��Ϣ�������

BOOL CUpnpMangerDlg::OnInitDialog()
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
	m_list.EnableWindow(FALSE);
	EnableOptButton(FALSE);
	SetDlgItemText(IDC_ST_ROUTER, "");
	m_ip.SetWindowText("239.255.255.250:1900");

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

VOID CUpnpMangerDlg::EnableOptButton(BOOL bEnabled /* = TRUE */)
{
	GetDlgItem(IDC_BT_REFURISH)->EnableWindow(bEnabled);
	GetDlgItem(IDC_BT_ADD)->EnableWindow(bEnabled);
	GetDlgItem(IDC_BT_DELETE)->EnableWindow(bEnabled);

}
void CUpnpMangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUpnpMangerDlg::OnPaint() 
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
HCURSOR CUpnpMangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpnpMangerDlg::OnBnClickedOk()
{
	if(FALSE != m_upnp.Search())
	{
		m_ip.EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST1)->EnableWindow(TRUE);
		EnableOptButton(TRUE);
		PostMessage(WM_COMMAND, IDC_BT_REFURISH);
	}
	else
	{
		MessageBox(m_upnp.GetLastError().GetBuffer(), NULL, MB_OK | MB_ICONERROR);
	}
}

void CUpnpMangerDlg::OnBnClickedBtRefurish()
{
	m_list.DeleteAllItems();
	if(m_list.GetHeaderCtrl()->GetItemCount() < 8)
	{
		m_list.InsertColumn(0, "���", 0, 30);
		m_list.InsertColumn(1, "ӳ���IP", 0, 100);
		m_list.InsertColumn(2, "�ڲ��˿�", 0, 50);
		m_list.InsertColumn(3, "Э��", 0, 50);
		m_list.InsertColumn(4, "�ⲿ����", 0, 100);
		m_list.InsertColumn(5, "ӳ��˿�", 0, 50);
		m_list.InsertColumn(6, "����", 0, 100);
		m_list.InsertColumn(7, "״̬", 0, 40);
	}

	for(int i = 0; i < 100; i++)
	{
		CString		strRemoteHost, strProtocol, strInternalClient, strDescription;
		USHORT		nExternalPort, nInternalPort;
		bool		bEnable;
		CHAR		szNum[50]		= {0};

		if(false == m_upnp.GetGenericPortMappingEntry(i, &strRemoteHost
			, &nExternalPort, &strProtocol, &nInternalPort, &strInternalClient
			, &bEnable, &strDescription))
			break;
		int			nItem		= 0;

		nItem = m_list.InsertItem(i, itoa(i+1, szNum, 10));
		m_list.SetItemText(nItem, 1, strInternalClient);
		m_list.SetItemText(nItem, 2, itoa(nInternalPort, szNum, 10));
		m_list.SetItemText(nItem, 3, strProtocol);
		m_list.SetItemText(nItem, 4, strRemoteHost);
		m_list.SetItemText(nItem, 5, itoa(nExternalPort, szNum, 10));
		m_list.SetItemText(nItem, 6, strDescription);
		m_list.SetItemText(nItem, 7, bEnable?"����":"����");
	}
}

void CUpnpMangerDlg::OnBnClickedBtAdd()
{
	CAddUpnpDlg		dlg(&m_upnp);

	if(IDOK == dlg.DoModal())
		PostMessage(WM_COMMAND, IDC_BT_REFURISH);
}

void CUpnpMangerDlg::OnBnClickedBtDelete()
{
	int		nSel;

	nSel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if(-1 == nSel)
	{
		MessageBox("��ѡ��Ҫɾ������");
		return;
	}

	MyUPnP::UPNPNAT_MAPPING		map;

	map.description = m_list.GetItemText(nSel, 6);
	map.externalPort = atoi(m_list.GetItemText(nSel, 5));
	map.internalPort = atoi(m_list.GetItemText(nSel, 2));
	map.protocol = ("TCP" == m_list.GetItemText(nSel, 3))?MyUPnP::UNAT_TCP:MyUPnP::UNAT_UDP;
	if(true == m_upnp.deletePortmap(atoi(m_list.GetItemText(nSel, 5)), m_list.GetItemText(nSel, 3)))
		PostMessage(WM_COMMAND, IDC_BT_REFURISH);
	else
		MessageBox(m_upnp.GetLastError(), NULL, MB_OK|MB_ICONERROR);
}
