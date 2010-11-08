// TestDeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <Winioctl.h>
#include "TestDevice.h"
#include "TestDeviceDlg.h"
#include ".\testdevicedlg.h"
#include "USBInfo.h"

#pragma comment(lib, "Setupapi.lib")

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


// CTestDeviceDlg 对话框



CTestDeviceDlg::CTestDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDeviceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDeviceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CTestDeviceDlg 消息处理程序

BOOL CTestDeviceDlg::OnInitDialog()
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
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CTestDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDeviceDlg::OnPaint() 
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
HCURSOR CTestDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 得到未分区的磁盘空间 
void CTestDeviceDlg::OnBnClickedButton1()
{
	HANDLE		hFile		= NULL;

	ASSERT(IsUserAnAdmin());
	hFile = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(INVALID_HANDLE_VALUE != hFile);
	// 获取磁盘容量
	GET_LENGTH_INFORMATION		gli			= {0};
	DWORD						dwRet		= 0;
	OVERLAPPED					ol			= {0};
	CString						str;

	DeviceIoControl(hFile, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0
		, &gli, sizeof(gli), &dwRet, NULL);
	str.Format("Disk length is %I64uGB.", gli.Length.QuadPart / 1000 / 1000 / 1000);
	MessageBox(str);

    CloseHandle(hFile);
}

void CTestDeviceDlg::OnBnClickedButton2()
{
// 	AfxMessageBox(GetDiskInfo("F:"));
// 	AfxMessageBox(GetDiskInfo("g:"));
// 	AfxMessageBox(GetDiskInfo("h:"));
	AfxMessageBox(GetDiskInfo("USB#ROOT_HUB20#4&23d8429a&0#{f18a0e88-c30c-11d0-8815-00a0c906bed8}"));
	AfxMessageBox(GetDiskInfo("Volume{2e17c581-6c4a-11df-93d2-001d0977a0c7}"));
// 	AfxMessageBox(GetDiskInfo("H:"));
// 	AfxMessageBox(GetDiskInfo("J:"));
// 	CHAR		szPath[100][1024]	= {0};

// 	GetDevicePath((LPGUID)&DiskClassGuid, (LPTSTR *)szPath);
// 	AfxMessageBox(szPath[0]);
	DWORD		szSerialNumber[100]		= {0};

// 	GetVolumeInformation("i:\\", NULL, 0, szSerialNumber, NULL, NULL, NULL, 0);
// 	GetVolumeInformation("f:\\", NULL, 0, szSerialNumber, NULL, NULL, NULL, 0);
// 	GetVolumeInformation("x:\\", NULL, 0, szSerialNumber, NULL, NULL, NULL, 0);
// 	GetVolumeInformation("g:\\", NULL, 0, szSerialNumber, NULL, NULL, NULL, 0);
// 	GetVolumeInformation("h:\\", NULL, 0, szSerialNumber, NULL, NULL, NULL, 0);
// 	GetVolumeInformation("c:\\", NULL, 0, szSerialNumber, NULL, NULL, NULL, 0);
	int i = 0;
}

void CTestDeviceDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
}
