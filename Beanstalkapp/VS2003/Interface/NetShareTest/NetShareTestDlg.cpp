// NetShareTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetShareTest.h"
#include "NetShareTestDlg.h"
#include ".\netsharetestdlg.h"
#include <lm.h>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Netapi32.lib")
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


// CNetShareTestDlg 对话框



CNetShareTestDlg::CNetShareTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetShareTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetShareTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetShareTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CNetShareTestDlg 消息处理程序

BOOL CNetShareTestDlg::OnInitDialog()
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
	CListCtrl *list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	list->InsertColumn(0, _T("共享名"), 0, 80);
	list->InsertColumn(1, _T("路径"), 0, 100);
	list->InsertColumn(2, _T("共享类型"), 0, 120);
	list->InsertColumn(3, _T("备注"), 0, 130);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CNetShareTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNetShareTestDlg::OnPaint() 
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
HCURSOR CNetShareTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNetShareTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	list->DeleteAllItems();

	PSHARE_INFO_502 BufPtr,p;
	NET_API_STATUS res;
	LPTSTR   lpszServer = NULL;
	DWORD er=0,tr=0,resume=0, i;
	std::map<DWORD, CString> lpTypes;
	lpTypes[STYPE_DISKTREE]		= _T("共享目录");
	lpTypes[STYPE_PRINTQ]		= _T("Print Queue");
	lpTypes[STYPE_DEVICE]		= _T("Communication device");
	lpTypes[STYPE_IPC]			= _T("Interprocess communication (IPC)");
	lpTypes[STYPE_SPECIAL]		= _T("默认共享");
	
	do // begin do
	{
		res = NetShareEnum (NULL, 502, (LPBYTE *) &BufPtr, -1, &er, &tr, &resume);
		if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
		{
			p=BufPtr;
			for(i=1;i<=er;i++)
			{
				int n = list->InsertItem(0xffff, CString(p->shi502_netname));
				list->SetItemText(n, 1, CString(p->shi502_path));
				list->SetItemText(n, 2, lpTypes[p->shi502_type]);
				list->SetItemText(n, 3, CString(p->shi502_remark));
				p++;
			}
			NetApiBufferFree(BufPtr);
		}
	}while(res==ERROR_MORE_DATA);
}

void CNetShareTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	for(int i = list->GetItemCount() - 1; i >= 0; i--)
	{
		if(list->GetCheck(i))	//删除共享
		{
			NetShareDel(NULL, CStringW(list->GetItemText(i, 0)).GetBuffer(), 0);
		}
	}

	OnBnClickedButton1();
}

void CNetShareTestDlg::OnBnClickedButton3()
{
	// 添加共享
	TCHAR				path[MAX_PATH]		= {0};
	TCHAR				name[40]			= {0};
	TCHAR				*lpPath				= NULL;
	SHARE_INFO_502		pShare				= {0};
	DWORD				error				= 0;
	LPITEMIDLIST		pIdl				= NULL;
	BROWSEINFO			bi					= {0};
	CStringW			wstrPath;
	CStringW			wstrName;

	//选择文件夹
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner		= GetSafeHwnd();
	bi.pszDisplayName	= name;
	bi.lpszTitle		= _T("选择共享目录");
	bi.ulFlags			= BIF_USENEWUI;
	pIdl = SHBrowseForFolder(&bi);
	if(NULL == pIdl)
	{
		AfxMessageBox(_T("选择目录失败"));
		return;
	}
	SHGetPathFromIDList(pIdl, path);
	lpPath = _tcsrchr(path, '\\');
	if(NULL != lpPath)
		_tcscpy(name, lpPath + 1);

	wstrPath = CStringW(path);
	wstrName = CStringW(name);

	pShare.shi502_path			= wstrPath.GetBuffer();
	pShare.shi502_netname		= wstrName.GetBuffer();
	pShare.shi502_type			= STYPE_DISKTREE;
	pShare.shi502_remark		= L"这是由NetShareTest添加的共享";
	pShare.shi502_max_uses		= 4;
	pShare.shi502_permissions	= ACCESS_READ; //权限
	pShare.shi502_current_uses	= 0;
	pShare.shi502_passwd		= NULL;
	pShare.shi502_reserved		= 0;

	if( NERR_Success == NetShareAdd(NULL, 502, (LPBYTE)&pShare, &error) )
		AfxMessageBox(_T("添加成功"));
	else
		AfxMessageBox(_T("添加失败"));

	OnBnClickedButton1();
}
