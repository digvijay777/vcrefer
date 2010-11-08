// NetShareTestDlg.cpp : ʵ���ļ�
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


// CNetShareTestDlg �Ի���



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


// CNetShareTestDlg ��Ϣ�������

BOOL CNetShareTestDlg::OnInitDialog()
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
	CListCtrl *list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	list->InsertColumn(0, _T("������"), 0, 80);
	list->InsertColumn(1, _T("·��"), 0, 100);
	list->InsertColumn(2, _T("��������"), 0, 120);
	list->InsertColumn(3, _T("��ע"), 0, 130);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNetShareTestDlg::OnPaint() 
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
	lpTypes[STYPE_DISKTREE]		= _T("����Ŀ¼");
	lpTypes[STYPE_PRINTQ]		= _T("Print Queue");
	lpTypes[STYPE_DEVICE]		= _T("Communication device");
	lpTypes[STYPE_IPC]			= _T("Interprocess communication (IPC)");
	lpTypes[STYPE_SPECIAL]		= _T("Ĭ�Ϲ���");
	
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
		if(list->GetCheck(i))	//ɾ������
		{
			NetShareDel(NULL, CStringW(list->GetItemText(i, 0)).GetBuffer(), 0);
		}
	}

	OnBnClickedButton1();
}

void CNetShareTestDlg::OnBnClickedButton3()
{
	// ��ӹ���
	TCHAR				path[MAX_PATH]		= {0};
	TCHAR				name[40]			= {0};
	TCHAR				*lpPath				= NULL;
	SHARE_INFO_502		pShare				= {0};
	DWORD				error				= 0;
	LPITEMIDLIST		pIdl				= NULL;
	BROWSEINFO			bi					= {0};
	CStringW			wstrPath;
	CStringW			wstrName;

	//ѡ���ļ���
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner		= GetSafeHwnd();
	bi.pszDisplayName	= name;
	bi.lpszTitle		= _T("ѡ����Ŀ¼");
	bi.ulFlags			= BIF_USENEWUI;
	pIdl = SHBrowseForFolder(&bi);
	if(NULL == pIdl)
	{
		AfxMessageBox(_T("ѡ��Ŀ¼ʧ��"));
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
	pShare.shi502_remark		= L"������NetShareTest��ӵĹ���";
	pShare.shi502_max_uses		= 4;
	pShare.shi502_permissions	= ACCESS_READ; //Ȩ��
	pShare.shi502_current_uses	= 0;
	pShare.shi502_passwd		= NULL;
	pShare.shi502_reserved		= 0;

	if( NERR_Success == NetShareAdd(NULL, 502, (LPBYTE)&pShare, &error) )
		AfxMessageBox(_T("��ӳɹ�"));
	else
		AfxMessageBox(_T("���ʧ��"));

	OnBnClickedButton1();
}
