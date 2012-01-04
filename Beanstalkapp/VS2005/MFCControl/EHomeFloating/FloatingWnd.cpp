#include "stdafx.h"
#include "FloatingWnd.h"

#ifndef GetRectWidth
#define GetRectWidth(x)			( (x).right - (x).left )
#define GetRectHeight(x)		( (x).bottom - (x).top )
#define GetRectWidthPtr(x)		( (x)->right - (x)->left )
#define GetRectHeightPtr(x)		( (x)->bottom - (x)->top )
#endif // GetRectWidth

#ifndef arrayof
#define arrayof(x)		( sizeof((x)) / sizeof((x)[0]) )
#endif

CFloatingWnd::CFloatingWnd(Gdiplus::Image* pImage, int nSplit)
:CWindowImpl<CFloatingWnd>()
{
	if(0 == nSplit)
		nSplit = 1;
	m_pBkImage = pImage;
	if(NULL != m_pBkImage)
	{
		m_size.cx = m_pBkImage->GetWidth() / nSplit;
		m_size.cy = m_pBkImage->GetHeight();
	}
	m_nBkImageIndex = 0;
	memset(m_szShowText, 0, sizeof(m_szShowText));
	memset(&m_rtClient, 0, sizeof(m_rtClient));
}

CFloatingWnd::~CFloatingWnd(void)
{
}

DWORD CFloatingWnd::GetWndStyle(DWORD dwStyle)
{
	dwStyle = CWindowImpl<CFloatingWnd>::GetWndStyle(dwStyle);
	dwStyle &= ~WS_CAPTION;
	return dwStyle;
}

DWORD CFloatingWnd::GetWndExStyle(DWORD dwStyle)
{
	dwStyle |= WS_EX_TOOLWINDOW;
	dwStyle |= WS_EX_TOPMOST;
	dwStyle |= WS_EX_LAYERED;
	return CWindowImpl<CFloatingWnd>::GetWndExStyle(dwStyle);
}

BOOL CFloatingWnd::UpdateWindow()
{
	BOOL		bRet;
	HDC			hDC;
	
	if(!IsWindow())
		return FALSE;
	bRet = CWindowImpl<CFloatingWnd>::UpdateWindow();
	hDC = GetDC();
	UpdateLayeredImageWindow(hDC);
	ReleaseDC(hDC);
	return bRet;
}

// 更新窗体
void CFloatingWnd::UpdateLayeredImageWindow(HDC hDC)
{
	HDC				hMemDC			= NULL;
	HBITMAP			hBitmap			= NULL;
	int				nSaveMemDC		= NULL;
	RECT			rtDC			= {0, 0, m_size.cx, m_size.cy};
	RECT			rtWnd			= {0};
	
	GetWindowRect(&rtWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rtWnd.right - rtWnd.left, rtWnd.bottom - rtWnd.top);
	nSaveMemDC = SaveDC(hMemDC);
	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, GetFont());
	// 绘制窗体
	OnLayerdPrint(hMemDC, &rtDC);
	// 更新窗体
	BLENDFUNCTION			blend		= {0};
	POINT					ptSrc		= {rtWnd.left, rtWnd.top};
	SIZE					szSrc		= {GetRectWidth(rtWnd), GetRectHeight(rtWnd)};	// 这个大小一定要是窗体的
	POINT					ptDes		= {0, 0};

	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	::UpdateLayeredWindow(m_hWnd, hDC, &ptSrc, &szSrc, hMemDC, &ptDes, 0, &blend, ULW_ALPHA);
	// 清理工作
	RestoreDC(hMemDC, nSaveMemDC);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
}
// 设置显示文本
void CFloatingWnd::SetShowText(LPCWSTR lpFmt, ...)
{
	va_list		vaArr;

	memset(m_szShowText, 0, sizeof(m_szShowText));
	va_start(vaArr, lpFmt);
	_vsnwprintf(m_szShowText, arrayof(m_szShowText)-1, lpFmt, vaArr);
	va_end(vaArr);

	UpdateWindow();
}
// 设置客户,除去影子部份的区域
void CFloatingWnd::SetClientRect(int x, int y, int w, int h)
{
	m_rtClient.left = x;
	m_rtClient.top = y;
	m_rtClient.right = m_rtClient.left + w;
	m_rtClient.bottom = m_rtClient.top + h;
}
// 初始化的位置
RECT CFloatingWnd::GetInitRect()
{
	RECT		rect;

	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rect, 0);
	rect.left = rect.right - GetRectWidth(m_rtClient);
	rect.top = rect.bottom - GetRectHeight(m_rtClient);
	rect.right = rect.left + m_size.cx;
	rect.bottom = rect.top + m_size.cy;
	return rect;
}
// 绘制函数
void CFloatingWnd::OnLayerdPrint(HDC hDC, LPRECT lpRect)
{
	Gdiplus::Graphics		graphics(hDC);
	Gdiplus::Font			font(L"宋体"/*L"Microsoft Sans Serif"*/, 9);
	Gdiplus::RectF			rect(lpRect->left + 86, lpRect->top + 9, GetRectWidthPtr(lpRect) - 86, GetRectHeightPtr(lpRect) - 9);
	Gdiplus::StringFormat	strfmt;
	Gdiplus::SolidBrush		brush(Gdiplus::Color(0xfe, 0xff, 0xff, 0xff));

	// 绘制背景
	graphics.DrawImage(m_pBkImage, 0, 0, m_nBkImageIndex * m_size.cx, 0, m_size.cx, m_size.cy, Gdiplus::UnitPixel);
	// 绘制文本
	strfmt.SetAlignment(Gdiplus::StringAlignmentCenter);
	graphics.DrawString(m_szShowText, wcslen(m_szShowText), &font, rect, &strfmt, &brush);	
}
// 让窗体每个地方都可以托动
LRESULT CFloatingWnd::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	RECT			rtWnd		= {0};
	POINT			pt			= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	bHandled = TRUE;
	GetWindowRect(&rtWnd);
	rtWnd.left += m_rtClient.left;
	rtWnd.top += m_rtClient.top;
	rtWnd.right = rtWnd.left + GetRectWidth(m_rtClient);
	rtWnd.bottom = rtWnd.top + GetRectHeight(m_rtClient);

	if(::PtInRect(&rtWnd, pt))
		return HTCAPTION;
	return HTTRANSPARENT;
}
// 拖动过程中移动窗体
LRESULT CFloatingWnd::OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	LPRECT		lpRect		= (LPRECT)lParam;

	bHandled = TRUE;
	MoveWindow(lpRect);
	return TRUE;
}
// 定时器操作
LRESULT CFloatingWnd::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(1 == wParam)
	{
		m_nBkImageIndex = GetTickCount() / 10000 % 3;
		SetShowText(L"今日使用: %02d时%02d分", GetTickCount() / 1000 / 60 % 100, GetTickCount() / 1000 % 60);
	}
	return 0;
}
// 创建完成操作
LRESULT CFloatingWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 测度，定时显示时间
	SetTimer(1, 1000, NULL);
	return 0;
}

LRESULT CFloatingWnd::OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	LPWINDOWPOS			lpWndPos		= (LPWINDOWPOS)lParam;
	RECT				rtScreen		= {0};

	// 让窗体在工作区内移动
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rtScreen, 0);
	if(lpWndPos->x > (rtScreen.right - m_rtClient.right))
	{
		lpWndPos->x = rtScreen.right - m_rtClient.right;
	}
	else if(lpWndPos->x < (0 - m_rtClient.left))
	{
		lpWndPos->x = 0 - m_rtClient.left;
	}
	if(lpWndPos->y > (rtScreen.bottom - m_rtClient.bottom))
	{
		lpWndPos->y = rtScreen.bottom - m_rtClient.bottom;
	}
	else if(lpWndPos->y < 0)
	{
		lpWndPos->y = 0;
	}

	return 0;
}
