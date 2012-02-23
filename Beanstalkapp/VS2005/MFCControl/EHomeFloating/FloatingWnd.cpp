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

CFloatingWnd::CFloatingWnd()
:CWnd()
{
	m_nBkImageIndex = 0;
	m_pBkImage = NULL;
	m_bAlpha = 255;
	memset(m_szShowText, 0, sizeof(m_szShowText));
	memset(&m_rtClient, 0, sizeof(m_rtClient));
	memset(&ClientMove, 0, sizeof(ClientMove));
}

CFloatingWnd::~CFloatingWnd(void)
{
}

BEGIN_MESSAGE_MAP(CFloatingWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CFloatingWnd::Create(Gdiplus::Image* pImage, int nSplit)
{
	if(NULL == pImage)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(0 == nSplit)
		nSplit = 1;
	m_pBkImage = pImage;
	if(NULL != m_pBkImage)
	{
		m_size.cx = m_pBkImage->GetWidth() / nSplit;
		m_size.cy = m_pBkImage->GetHeight();
	}
	// �������� ��ʽ����360
	return CWnd::CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR
		, NULL, NULL
		, /*WS_VISIBLE |*/ WS_CHILD | WS_CLIPSIBLINGS /*| WS_CLIPCHILDREN*/ | WS_POPUP | WS_OVERLAPPED
		, GetInitRect(), GetDesktopWindow()
		, 0, NULL);
}

void CFloatingWnd::UpdateWindow()
{
	CDC*		pDC;
	
	if(!IsWindow(GetSafeHwnd()))
		return;
	CWnd::UpdateWindow();

	pDC = GetDC();
	UpdateLayeredImageWindow(pDC->GetSafeHdc());
	ReleaseDC(pDC);
}

// ���´���
void CFloatingWnd::UpdateLayeredImageWindow(HDC hDC)
{
	HDC				hMemDC			= NULL;
	HBITMAP			hBitmap			= NULL;
	HBITMAP			hOldBitmap		= NULL;
	int				nSaveMemDC		= NULL;
	RECT			rtDC			= {0, 0, m_size.cx, m_size.cy};
	RECT			rtWnd			= {0};
	
	GetWindowRect(&rtWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rtWnd.right - rtWnd.left, rtWnd.bottom - rtWnd.top);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	nSaveMemDC = SaveDC(hMemDC);
	// ���ƴ���
	OnLayerdPrint(hMemDC, &rtDC);
	// ���´���
	BLENDFUNCTION			blend		= {0};
	POINT					ptSrc		= {rtWnd.left, rtWnd.top};
	SIZE					szSrc		= {GetRectWidth(rtWnd), GetRectHeight(rtWnd)};	// �����Сһ��Ҫ�Ǵ����
	POINT					ptDes		= {0, 0};

	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = m_bAlpha;
	if(FALSE == ::UpdateLayeredWindow(m_hWnd, hDC, &ptSrc, &szSrc, hMemDC, &ptDes, 0, &blend, ULW_ALPHA))
	{
		ModifyStyleEx(WS_EX_LAYERED, 0, 0);
		ModifyStyleEx(0, WS_EX_LAYERED, 0);
		::UpdateLayeredWindow(m_hWnd, hDC, &ptSrc, &szSrc, hMemDC, &ptDes, 0, &blend, ULW_ALPHA);
	}
	// ������
	RestoreDC(hMemDC, nSaveMemDC);
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
}
// ����͸����
void CFloatingWnd::SetWindowAlpha(BYTE bAlpha)
{
	m_bAlpha = bAlpha;
	UpdateWindow();
}
// ������ʾ�ı�
void CFloatingWnd::SetShowText(LPCWSTR lpFmt, ...)
{
	va_list		vaArr;

	memset(m_szShowText, 0, sizeof(m_szShowText));
	va_start(vaArr, lpFmt);
	_vsnwprintf(m_szShowText, arrayof(m_szShowText)-1, lpFmt, vaArr);
	va_end(vaArr);

	UpdateWindow();
}
// �����Ӱ�����ϲ��ֵĿ��
void CFloatingWnd::GetShadeFrm(int &nLeft, int &nTop)
{
	nLeft = m_rtClient.left;
	nTop = m_rtClient.top;
}
// ���ÿͻ�,��ȥӰ�Ӳ��ݵ�����
void CFloatingWnd::SetClientRect(int x, int y, int w, int h)
{
	m_rtClient.left = x;
	m_rtClient.top = y;
	m_rtClient.right = m_rtClient.left + w;
	m_rtClient.bottom = m_rtClient.top + h;
}
// ��ʼ����λ��
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
// ���ƺ���
void CFloatingWnd::OnLayerdPrint(HDC hDC, LPRECT lpRect)
{
	Gdiplus::Graphics		graphics(hDC);
	Gdiplus::Font			font(L"����"/*L"Microsoft Sans Serif"*/, 9);
	Gdiplus::RectF			rect((Gdiplus::REAL)lpRect->left + 86, (Gdiplus::REAL)lpRect->top + 9, (Gdiplus::REAL)GetRectWidthPtr(lpRect) - 86, (Gdiplus::REAL)GetRectHeightPtr(lpRect) - 9);
	Gdiplus::StringFormat	strfmt;
	Gdiplus::SolidBrush		brush(Gdiplus::Color(0xfe, 0xff, 0xff, 0xff));

	// ���Ʊ���
	graphics.DrawImage(m_pBkImage, 0, 0, m_nBkImageIndex * m_size.cx, 0, m_size.cx, m_size.cy, Gdiplus::UnitPixel);
	// �����ı�
	strfmt.SetAlignment(Gdiplus::StringAlignmentCenter);
	graphics.DrawString(m_szShowText, (int)wcslen(m_szShowText), &font, rect, &strfmt, &brush);	
}
// �ô���ÿ���ط��������ж�
LRESULT CFloatingWnd::OnNcHitTest(CPoint point)
{
	RECT			rtWnd		= {0};

	GetWindowRect(&rtWnd);
	rtWnd.left += m_rtClient.left;
	rtWnd.top += m_rtClient.top;
	rtWnd.right = rtWnd.left + GetRectWidth(m_rtClient);
	rtWnd.bottom = rtWnd.top + GetRectHeight(m_rtClient);

	if(::PtInRect(&rtWnd, point))
		return HTCLIENT;
	return HTTRANSPARENT;
}
// ������ɲ���
int CFloatingWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// ��ȣ���ʱ��ʾʱ��
	return 0;
}

void CFloatingWnd::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	RECT				rtScreen		= {0};

	// �ô����ڹ��������ƶ�
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rtScreen, 0);
	if(lpwndpos->x > (rtScreen.right - m_rtClient.right))
	{
		lpwndpos->x = rtScreen.right - m_rtClient.right;
	}
	else if(lpwndpos->x < (0 - m_rtClient.left))
	{
		lpwndpos->x = 0 - m_rtClient.left;
	}
	if(lpwndpos->y > (rtScreen.bottom - m_rtClient.bottom))
	{
		lpwndpos->y = rtScreen.bottom - m_rtClient.bottom;
	}
	else if(lpwndpos->y < 0)
	{
		lpwndpos->y = 0;
	}

	CWnd::OnWindowPosChanging(lpwndpos);
}


void CFloatingWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// �����˵���SetLayeredWindowAttributes��Ҫ���ر���
	UpdateWindow();
}

void CFloatingWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	ClientMove.move = TRUE;
	ClientMove.pt = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CFloatingWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(NULL != ClientMove.cursor)
		SetCursor(ClientMove.cursor);
	if(ClientMove.capture)
		ReleaseCapture();
	else///(!ClientMove.move)
		NotifyLButtonClick();

	memset(&ClientMove, 0, sizeof(ClientMove));
	CWnd::OnLButtonUp(nFlags, point);
}

void CFloatingWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(ClientMove.move)
	{
		CRect		rect;

		if(!ClientMove.capture)
		{
			ClientMove.capture = TRUE;
			SetCapture();
			ClientMove.cursor = SetCursor(::LoadCursor(NULL, IDC_SIZEALL));	
			NotifyMouseMove();
		}
		GetWindowRect(&rect);
		rect.MoveToX(rect.left + point.x - ClientMove.pt.x);
		rect.MoveToY(rect.top + point.y - ClientMove.pt.y);
		// �ƶ�����
		MoveWindow(&rect, FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);
}
