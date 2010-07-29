// UpnpMangerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpnpManger.h"
#include "UpnpMangerDlg.h"
#include ".\upnpmangerdlg.h"
#include "AddUpnpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CUpnpMangerDlg 对话框



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


// CUpnpMangerDlg 消息处理程序

BOOL CUpnpMangerDlg::OnInitDialog()
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
	m_list.EnableWindow(FALSE);
	EnableOptButton(FALSE);
	SetDlgItemText(IDC_ST_ROUTER, "");
	m_ip.SetWindowText("239.255.255.250:1900");

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUpnpMangerDlg::OnPaint() 
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
		m_list.InsertColumn(0, "编号", 0, 30);
		m_list.InsertColumn(1, "映射机IP", 0, 100);
		m_list.InsertColumn(2, "内部端口", 0, 50);
		m_list.InsertColumn(3, "协议", 0, 50);
		m_list.InsertColumn(4, "外部主机", 0, 100);
		m_list.InsertColumn(5, "映射端口", 0, 50);
		m_list.InsertColumn(6, "描述", 0, 100);
		m_list.InsertColumn(7, "状态", 0, 40);
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
		m_list.SetItemText(nItem, 7, bEnable?"启用":"禁用");
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
		MessageBox("请选择要删除的项");
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
