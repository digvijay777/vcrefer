// CreateRGBDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateRGB.h"
#include "CreateRGBDlg.h"
#include ".\creatergbdlg.h"

#include <vector>

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


// CCreateRGBDlg �Ի���



CCreateRGBDlg::CCreateRGBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateRGBDlg::IDD, pParent)
	, m_nModule(0)
	, m_path(_T(""))
	, m_nRows(0)
	, m_nAllScan(FALSE)
	, m_bEnter(FALSE)
	, m_nShowModule(2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateRGBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nModule);
	DDX_Text(pDX, IDC_EDIT2, m_path);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nRows);
	DDX_Check(pDX, IDC_CHECK2, m_nAllScan);
	DDX_Check(pDX, IDC_CHECK3, m_bEnter);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nShowModule);
}

BEGIN_MESSAGE_MAP(CCreateRGBDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_SCAN, OnBnClickedBtScan)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateRGBDlg ��Ϣ�������

BOOL CCreateRGBDlg::OnInitDialog()
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

void CCreateRGBDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCreateRGBDlg::OnPaint() 
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
HCURSOR CCreateRGBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateRGBDlg::OnBnClickedBtScan()
{
	CFileDialog			fDlg(TRUE, "*.bmp", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP �ļ� (*.bmp)|*.bmp||", this);

	if(fDlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT2, fDlg.GetPathName().GetBuffer());
	}
}

void CCreateRGBDlg::OnBnClickedOk()
{
	// ��ʼת��
	UpdateData();

	HBITMAP	hBit	= (HBITMAP)LoadImage(NULL, m_path.GetBuffer(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if(NULL == hBit)
	{
		AfxMessageBox("���ļ�ʧ��");
		return;
	}

	CBitmap	bit;
	bit.Attach(hBit);

	BITMAP bmp;
	bit.GetBitmap(&bmp);

	CDC		memDC;
	memDC.CreateCompatibleDC(GetDC());
	CBitmap*	pBitmap		= memDC.SelectObject(&bit);

	std::vector<COLORREF>	color;
	// ����
	if(m_nModule == 0) // ����
	{
		int		nRows = 1;
		if(m_nAllScan)
			nRows = bmp.bmWidth;
		for(int j = 0; j < nRows; j++)
		{
			for(int i = 0; i < bmp.bmHeight; i++)
			{
				color.push_back(memDC.GetPixel(j, i));
			}
		}
	}
	else // ����
	{
		int		nCols = 1;
		if(m_nAllScan)
			nCols = bmp.bmHeight;
		for(int j = 0; j < nCols; j++)
		{
			for(int i = 0; i < bmp.bmWidth; i++)
			{
				color.push_back(memDC.GetPixel(i, j));
			}
		}
	}
	// ��ʾ
	CString		strShow;

	for(int i = 0; i < color.size(); i++)
	{
		CString		strTmp;
		COLORREF	cl	= color[i];
		if(1 == m_nShowModule) // RGB��ʽ
		{
			strTmp.Format("RGB(0x%02x, 0x%02x, 0x%02x), ", GetRValue(cl), GetGValue(cl), GetBValue(cl)); 
		}
		else if(0 == m_nShowModule) // RGB����ʽ
		{
			strTmp.Format("{0x%02x, 0x%02x, 0x%02x}, ", GetRValue(cl), GetGValue(cl), GetBValue(cl));
		}
		else if(2 == m_nShowModule) // 16���̾���ʽ
		{
			strTmp.Format("0x%06x, ", cl);
		}
		else
		{
			continue;
		}

		strShow += strTmp;
		if((i + 1) % (m_nRows + 3) == 0)
		{
			if(m_bEnter)
				strShow += "\\\r\n";
			else
				strShow += "\r\n";
		}
	}

	SetDlgItemText(IDC_EDIT1, strShow.GetBuffer());

	// ����
	memDC.SelectObject(pBitmap);
	memDC.DeleteDC();
	bit.DeleteObject();

}
