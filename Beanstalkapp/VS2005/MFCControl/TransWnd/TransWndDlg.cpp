// TransWndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TransWnd.h"
#include "TransWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransWndDlg 对话框




CTransWndDlg::CTransWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImage = NULL;
}

void CTransWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransWndDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CTransWndDlg 消息处理程序
BOOL CTransWndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CComPtr<IStream>		spStream;
	HRSRC					hRsrc			= FindResource(NULL, _T("bg.png"), _T("IDR_PNG"));
	HGLOBAL					hGlobal			= NULL;
	DWORD					dwSize			= SizeofResource(NULL, hRsrc);

	hGlobal = LoadResource(NULL, hRsrc);
	if(NULL != hGlobal)
	{
		CreateStreamOnHGlobal(NULL, TRUE, &spStream);
		if(NULL != spStream)
		{
			spStream->Write(LockResource(hGlobal), dwSize, NULL);
			m_pImage = Gdiplus::Image::FromStream(spStream);
			UnlockResource(hGlobal);
		}
		FreeResource(hGlobal);
	}
	if(NULL == m_pImage)
		return	TRUE;
	// 创建形窗体
	CDC*		pDC		= GetDC();

	UpdateLayeredImageWindow(m_pImage, pDC, this/*&m_dlgShow*/);
	ReleaseDC(pDC);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CTransWndDlg::UpdateLayeredImageWindow(Gdiplus::Image* pImage, CDC* pDC, CWnd* pWnd)
{
	CRect			rect;
	CDC				memDC;
	CBitmap			bitmap;
	int				nSaveDC;
	DWORD			dwStyleEx;

	if(NULL == pImage || !IsWindow(pWnd->GetSafeHwnd()) || NULL == pDC)
		return FALSE;
	// 检测窗体属性
	dwStyleEx = GetWindowLong(pWnd->GetSafeHwnd(), GWL_EXSTYLE);
	if(WS_EX_LAYERED != (dwStyleEx & WS_EX_LAYERED))
	{
		SetWindowLong(pWnd->GetSafeHwnd(), GWL_EXSTYLE, dwStyleEx | WS_EX_LAYERED);
	}
	// 根据图片设置窗体大小
	pWnd->SetWindowPos(NULL, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	// 创建DC
	pWnd->GetWindowRect(&rect);
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	if(NULL == bitmap.GetSafeHandle() || NULL == memDC.GetSafeHdc())
		return FALSE;
	nSaveDC = memDC.SaveDC();
	memDC.SelectObject(&bitmap);
	// 绘制图片
	Gdiplus::Graphics		graphics(memDC.GetSafeHdc());

	graphics.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
	graphics.ReleaseHDC(memDC.GetSafeHdc());
	// 创建透明窗体
	BLENDFUNCTION			blend		= {0};
	POINT					ptSrc		= rect.TopLeft();
	SIZE					szSrc		= {rect.Width(), rect.Height()};	// 这个大小一定要是窗体的
	POINT					ptDes		= {0, 0};

	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	pWnd->UpdateLayeredWindow(pDC, &ptSrc, &szSrc, &memDC, &ptDes, 0, &blend, ULW_ALPHA);
	// 释放资源
	memDC.RestoreDC(nSaveDC);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTransWndDlg::OnPaint()
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
//
HCURSOR CTransWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTransWndDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;

	return CDialog::OnNcHitTest(point);
}

LRESULT CTransWndDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_PAINT == message)
	{
		CPaintDC dc(this);

		dc.FillSolidRect(0, 0, 100, 100, RGB(0xff, 0x0, 0x0));
		return 0;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
