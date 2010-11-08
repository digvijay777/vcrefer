// SystemInfoDlg.cpp : ʵ���ļ�
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


// CSystemInfoDlg �Ի���



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


// CSystemInfoDlg ��Ϣ�������

BOOL CSystemInfoDlg::OnInitDialog()
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
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck((ComReg::IsIEStartPageEnabled())?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck((ComReg::IsRestrictGpedit())?BST_CHECKED:BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck((ComReg::IsRegistryToolEnabled())?BST_CHECKED:BST_UNCHECKED);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSystemInfoDlg::OnPaint() 
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
	// ����ʱ��
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

	str.Format("����������Ŀ:%d\n", run.GetCount());
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

	str.Format("�ҵ������ļ�: %d��\n�����ļ��ܴ�С: %d"
		, clean.GetCount()
		, clean.GetAllSize());
	AfxMessageBox(str.GetBuffer());
}

void CSystemInfoDlg::OnBnClickedButton4()
{
	// ��������
	WIN32_FIND_DATAW	fd;
	HANDLE				hFile		= FindFirstFileW(L"\\\\?\\UNC\\192.168.1.254\\webroot\\*", &fd);

	do 
	{
		INT I = 0;
	} while (FindNextFileW(hFile, &fd));
	FindClose(hFile);
}
