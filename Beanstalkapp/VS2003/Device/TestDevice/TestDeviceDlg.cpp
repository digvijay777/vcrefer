// TestDeviceDlg.cpp : ʵ���ļ�
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


// CTestDeviceDlg �Ի���



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


// CTestDeviceDlg ��Ϣ�������

BOOL CTestDeviceDlg::OnInitDialog()
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestDeviceDlg::OnPaint() 
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
HCURSOR CTestDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// �õ�δ�����Ĵ��̿ռ� 
void CTestDeviceDlg::OnBnClickedButton1()
{
	HANDLE		hFile		= NULL;

	ASSERT(IsUserAnAdmin());
	hFile = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(INVALID_HANDLE_VALUE != hFile);
	// ��ȡ��������
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
