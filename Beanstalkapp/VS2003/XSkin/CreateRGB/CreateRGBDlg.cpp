// CreateRGBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateRGB.h"
#include "CreateRGBDlg.h"
#include ".\creatergbdlg.h"

#include <vector>

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


// CCreateRGBDlg 对话框



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


// CCreateRGBDlg 消息处理程序

BOOL CCreateRGBDlg::OnInitDialog()
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCreateRGBDlg::OnPaint() 
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
HCURSOR CCreateRGBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateRGBDlg::OnBnClickedBtScan()
{
	CFileDialog			fDlg(TRUE, "*.bmp", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP 文件 (*.bmp)|*.bmp||", this);

	if(fDlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT2, fDlg.GetPathName().GetBuffer());
	}
}

void CCreateRGBDlg::OnBnClickedOk()
{
	// 开始转换
	UpdateData();

	HBITMAP	hBit	= (HBITMAP)LoadImage(NULL, m_path.GetBuffer(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if(NULL == hBit)
	{
		AfxMessageBox("打开文件失败");
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
	// 操作
	if(m_nModule == 0) // 纵向
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
	else // 横向
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
	// 显示
	CString		strShow;

	for(int i = 0; i < color.size(); i++)
	{
		CString		strTmp;
		COLORREF	cl	= color[i];
		if(1 == m_nShowModule) // RGB格式
		{
			strTmp.Format("RGB(0x%02x, 0x%02x, 0x%02x), ", GetRValue(cl), GetGValue(cl), GetBValue(cl)); 
		}
		else if(0 == m_nShowModule) // RGB矩阵式
		{
			strTmp.Format("{0x%02x, 0x%02x, 0x%02x}, ", GetRValue(cl), GetGValue(cl), GetBValue(cl));
		}
		else if(2 == m_nShowModule) // 16进程矩阵式
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

	// 结束
	memDC.SelectObject(pBitmap);
	memDC.DeleteDC();
	bit.DeleteObject();

}
