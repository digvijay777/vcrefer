// SystemInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemInfo.h"
#include "SystemInfoDlg.h"
#include ".\systeminfodlg.h"
#include "System\SysAutoRun.h"
#include "System\ComReg.h"
#include "System\SysCleanTemp.h"

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


// CSystemInfoDlg 对话框



CSystemInfoDlg::CSystemInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSystemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSystemInfoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CSystemInfoDlg 消息处理程序

BOOL CSystemInfoDlg::OnInitDialog()
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
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck((ComReg::IsIEStartPageEnabled())?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck((ComReg::IsRestrictGpedit())?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck((ComReg::IsRegistryToolEnabled())?BST_CHECKED:BST_UNCHECKED);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CSystemInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSystemInfoDlg::OnPaint() 
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
HCURSOR CSystemInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
#include <pdh.h>
#include <pdhmsg.h>

#pragma comment(lib, "Pdh.lib")

void CSystemInfoDlg::OnBnClickedButton1()
{
	// 空闲时间
	HQUERY          hQuery;
	PDH_STATUS      pdhStatus;
	DWORD           dwFormat = PDH_FMT_DOUBLE; 
	PDH_FMT_COUNTERVALUE  *lpItemBuffer;
	HCOUNTER        hCounter;
	CHAR            szCounterPath[45] = 
		TEXT("\\Processor(0)\\% Processor Time");

	// Open a query object.
	pdhStatus = PdhOpenQuery (NULL,
		0,
		&hQuery);

	// Add the counter that created the data in the log file.
	pdhStatus = PdhAddCounter (hQuery,
		szCounterPath,
		0,
		&hCounter);

	// Allocate the counter value structure.
	lpItemBuffer = (PDH_FMT_COUNTERVALUE *) GlobalAlloc 
		(GPTR, sizeof(PDH_FMT_COUNTERVALUE));
	// Read the performance data records.
	pdhStatus = PdhCollectQueryData(hQuery);
	while (pdhStatus == ERROR_SUCCESS) { 

		// Format the performance data record.
		pdhStatus = PdhGetFormattedCounterValue (hCounter,
			dwFormat,
			(LPDWORD)NULL,
			lpItemBuffer);

		// Print the performance data record.
		TRACE(TEXT("Log Record Value = %4.8f\n"), 
			lpItemBuffer->doubleValue);

		// Read the next record
		pdhStatus = PdhCollectQueryData(hQuery);
		Sleep(1);

	}

	// Close the query and the log file.
	pdhStatus = PdhCloseQuery(hQuery);

}

void CSystemInfoDlg::OnBnClickedButton2()
{
	CSysAutoRun		run;

	run.Scan();

	CString			str;

	str.Format("启动项有项目:%d\n", run.GetCount());
	for(int i = 0; i < run.GetCount(); i++)
	{
		str += run.m_vctAutoRun[i].sName.c_str();
		str += ":";
		str += run.m_vctAutoRun[i].sCommand.c_str();
		str += "\n";
	}
	AfxMessageBox(str.GetBuffer());
}

void CSystemInfoDlg::OnBnClickedButton3()
{

}

void CSystemInfoDlg::OnBnClickedCheck1()
{
	if(BST_CHECKED == ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		ComReg::SetIEStartPageEnabled(TRUE);
	}
	else
	{
		ComReg::SetIEStartPageEnabled(FALSE);
	}

	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck((ComReg::IsIEStartPageEnabled())?BST_CHECKED:BST_UNCHECKED);
}

void CSystemInfoDlg::OnBnClickedCheck2()
{
	if(BST_CHECKED == ((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		ComReg::SetRestrictGpedit(TRUE);
	}
	else
	{
		ComReg::SetRestrictGpedit(FALSE);
	}

	((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(ComReg::IsRestrictGpedit()?BST_CHECKED:BST_UNCHECKED);
}

void CSystemInfoDlg::OnBnClickedCheck3()
{
	if(BST_CHECKED == ((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		ComReg::SetRegistryToolEnabled(TRUE);
	}
	else
	{
		ComReg::SetRegistryToolEnabled(FALSE);
	}

	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(ComReg::IsRegistryToolEnabled()?BST_CHECKED:BST_UNCHECKED);
}

void CSystemInfoDlg::OnBnClickedButton5()
{
	CSysCleanTemp		clean;

	clean.Scan();

	CString			str;

	str.Format("找到垃圾文件: %d个\n垃圾文件总大小: %d"
		, clean.GetCount()
		, clean.GetAllSize());
	AfxMessageBox(str.GetBuffer());
}

void CSystemInfoDlg::OnBnClickedButton4()
{
	// 查找名字
	WIN32_FIND_DATAW	fd;
	HANDLE				hFile		= FindFirstFileW(L"\\\\?\\UNC\\192.168.1.254\\webroot\\*", &fd);

	do 
	{
		INT I = 0;
	} while (FindNextFileW(hFile, &fd));
	FindClose(hFile);
}
