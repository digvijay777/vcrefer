/*
 *	图片窗口框架
 */
#include "stdafx.h"
#include "XSkinWnd.h"

// 边框颜色
#ifndef COLOR_XSKIN_INNERFRAME
	#define COLOR_XSKIN_OUTERFRAME						RGB(0x6b, 0x6e, 0x71)
	#define COLOR_XSKIN_INNERFRAME						RGB(0x99, 0x99, 0x99)		
#endif

// 标题栏背景
#define COLOR_XSKIN_TITLEBARBK()\
	{\
	RGB(0x83, 0x86, 0x89), RGB(0x80, 0x83, 0x86), RGB(0x7c, 0x7f, 0x81), RGB(0x76, 0x79, 0x7c), \
	RGB(0x72, 0x76, 0x79), RGB(0x6c, 0x6f, 0x72), RGB(0x68, 0x6c, 0x6f), RGB(0x63, 0x67, 0x6a), \
	RGB(0x5e, 0x62, 0x65), RGB(0x5a, 0x5e, 0x61), RGB(0x55, 0x59, 0x5c), RGB(0x52, 0x56, 0x59), \
	RGB(0x4d, 0x51, 0x54), RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), \
	RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), \
	RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), RGB(0x32, 0x37, 0x3a), \
	RGB(0x32, 0x37, 0x3a) \
	}\


//////////////////////////////////////////////////////////////////////////
CXSkinWnd::CXSkinWnd(CWnd* pWnd,UINT uidCaption)
{
	m_pSkinWnd = pWnd;
	m_uidCaption = uidCaption;
	m_bReDarw = FALSE;
	m_bNcLButtonDown = FALSE;
	m_nTitleHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXFRAME);
}

CXSkinWnd::~CXSkinWnd()
{
}

/*
 *	窗口过程
 *  返回值:TRUE 不再执行默认过程 FALSE 需要执行默认过程
 */
BOOL CXSkinWnd::SkinWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plRet)
{
	ASSERT(plRet != NULL);
	ASSERT(m_pSkinWnd && IsWindow(m_pSkinWnd->m_hWnd));

	if(WM_CREATE == message)
	{
		return OnSkinCreateWnd(wParam, lParam, plRet);
	}
	
	if(FALSE == m_bReDarw)
		return FALSE;		// 不需要重绘

	if(WM_NCCALCSIZE == message)
	{
		return OnSkinNccalSize(wParam, lParam, plRet);
	}
	else if(WM_NCPAINT == message || WM_NCACTIVATE == message)
	{
		return OnSkinNcPaint(wParam, lParam, plRet);
	}
	else if(WM_NCLBUTTONDOWN == message)
	{
		return OnSkinNcLButtonDown(wParam, lParam, plRet);
	}
	else if(WM_NCHITTEST == message)
	{
		return OnSkinNcHitTest(wParam, lParam, plRet);
	}
	else if(m_bNcLButtonDown && (WM_NCLBUTTONUP == message || WM_LBUTTONUP == message))
	{
		return OnSkinNcLbuttonUp(wParam, lParam, plRet);
	}
	else if(WM_NCRBUTTONUP == message)
	{
		return OnSkinNcRbuttonUp(wParam, lParam, plRet);
	}
	else if(WM_NCMOUSEMOVE == message)
	{
		return OnSkinNcMouseMove(wParam, lParam, plRet);
	}
	else if(WM_SIZE == message)
	{
		// 创建圆角
		SetWindowCirRgn();
	}

	return FALSE;
}

/*
 *	创建窗体时
 */
BOOL CXSkinWnd::OnSkinCreateWnd(WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if(0 == m_uidCaption)
		return FALSE;

	ASSERT(m_pSkinWnd && IsWindow(m_pSkinWnd->m_hWnd));
	// 去除标题栏
	m_bReDarw = TRUE;
	DWORD dwStyle = ::GetWindowLong((HWND)m_pSkinWnd->m_hWnd, GWL_STYLE);
	dwStyle &= ~WS_CAPTION;
	::SetWindowLong(m_pSkinWnd->m_hWnd, GWL_STYLE, dwStyle);

	return FALSE;
}

/*
 *	创建圆角
 */
VOID CXSkinWnd::SetWindowCirRgn()
{
	RECT rect;
	m_pSkinWnd->GetWindowRect(&rect);
	RECT nRect = {0, 0, rect.right - rect.left, rect.bottom - rect.top};
	RECT nRect2 = {0, 6, rect.right - rect.left, rect.bottom - rect.top};
	CRgn rgn, rgn2;
	rgn.CreateRoundRectRgn(nRect.left, nRect.top, nRect.right, nRect.bottom, 9, 9);	
	rgn2.CreateRectRgn(nRect2.left, nRect2.top, nRect2.right, nRect2.bottom);
	rgn.CombineRgn(&rgn, &rgn2, RGN_OR);
	rgn2.DeleteObject();
	m_pSkinWnd->SetWindowRgn(rgn, true);
	rgn.DeleteObject();

	m_pSkinWnd->Invalidate();
}
/*
 *	调整大小
 */
BOOL CXSkinWnd::OnSkinNccalSize(WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if(0 != wParam)
	{
		NCCALCSIZE_PARAMS *param = (NCCALCSIZE_PARAMS *)lParam;
		param->rgrc[0].top += 27 - GetSystemMetrics(SM_CYFIXEDFRAME);  
		param->rgrc[0].bottom -= 2 - GetSystemMetrics(SM_CYFIXEDFRAME);   
		param->rgrc[0].left += 2 - GetSystemMetrics(SM_CXFIXEDFRAME);   
		param->rgrc[0].right -= 2 - GetSystemMetrics(SM_CYFIXEDFRAME );  
	}

	return FALSE;
}

/*
 *	非客户区绘制
 */
#define IMAGE_TITLEBK_HEIGHT		27	//图片高度为27像素
#define TITLE_BUTTON_Y				4 //关闭、最大化按钮的上边界
#define TITLE_BUTTON_X				7 //关闭、最大化按钮的右边界
#define FRAME_BORDER_BUTTON			2 //下边界
#define FRAME_BORDER_LEFT			2 //左边界
#define FRAME_BORDER_RIGHT			2 //右边界

BOOL CXSkinWnd::OnSkinNcPaint(WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if(FALSE == m_bReDarw)
		return FALSE;

	ASSERT(m_pSkinWnd && IsWindow(m_pSkinWnd->m_hWnd));

	CDC *pdc = m_pSkinWnd->GetWindowDC();
	if(NULL == pdc)	//得到DC失败
		return false;
	CRect mainRect;
	m_pSkinWnd->GetWindowRect(&mainRect);
	//////////////////////////////////////////////////////////////////////////////////////
	// 绘制标题样背景
	DrawWndTitleBarBk(pdc, &mainRect);

	// 绘制边框
	DrawWndFrame(pdc, mainRect);
	::ReleaseDC(m_pSkinWnd->m_hWnd, pdc->m_hDC);

	return TRUE;
}

/*
 *	 
 */
BOOL CXSkinWnd::OnSkinNcLButtonDown(WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	POINT		pt		= {LOWORD(lParam), HIWORD(lParam)};

// 	if(PtInCaption(pt))
// 	{
// 
// 	}
	if(HTCLOSE == wParam)
	{
		
		return TRUE;
	}
	return FALSE;
}

BOOL CXSkinWnd::OnSkinNcHitTest(WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	ASSERT(m_pSkinWnd && IsWindow(m_pSkinWnd->m_hWnd));

	POINT		pt		= {LOWORD(lParam), HIWORD(lParam)};
	RECT		rect;

	m_pSkinWnd->GetWindowRect(&rect);
	if(PtInClose(&rect, pt))
	{
		*lpResult = HTCLOSE;
		return TRUE;
	}
	else if(PtInCaption(&rect, pt))
	{
		*lpResult = HTCAPTION;
		return TRUE;
	}
	return FALSE;
}

BOOL CXSkinWnd::OnSkinNcLbuttonUp(WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

/*
 *	点坐标是否在标题栏内
 */
BOOL CXSkinWnd::PtInCaption(const RECT* rect, POINT pt)
{
	RECT		capRect		= *rect;

	capRect.left += 2;
	capRect.right -= 2;
	capRect.top += 2;
	capRect.bottom = capRect.top + m_nTitleHeight;
	if(::PtInRect(&capRect, pt))
		return TRUE;
	
	return FALSE;
}
BOOL CXSkinWnd::PtInClose(const RECT* rect, POINT pt)
{
	RECT		clsRect			= *rect;

	clsRect.right -= 10;
	clsRect.top += 2;
	clsRect.bottom = clsRect.top + 20;
	clsRect.left = clsRect.right - 100;
	if(::PtInRect(&clsRect, pt))
		return TRUE;

	return FALSE;
}
/*
 *	系统菜单
 */
BOOL CXSkinWnd::OnSkinNcRbuttonUp(WPARAM wParam, LPARAM lParam, LRESULT* lpResult /* = NULL */)
{
	POINT		pt		= {LOWORD(lParam), HIWORD(lParam)};

	if(HTCAPTION == wParam)
	{
		CMenu*		pSysMenu	= m_pSkinWnd->GetSystemMenu(FALSE);
		if(NULL != pSysMenu)
		{
			DWORD	dwMenuID		= 0;
			dwMenuID = pSysMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, m_pSkinWnd);
			m_pSkinWnd->SendMessage(WM_SYSCOMMAND, dwMenuID, 0);
			return TRUE;
		}
	}
	return FALSE;
}

/*
 *	移动鼠标
 */
BOOL CXSkinWnd::OnSkinNcMouseMove(WPARAM wParam, LPARAM lParam, LRESULT* lpResult /* = NULL */)
{
	if(HTCLOSE == wParam)
	{
		TRACE("OnSkinNcMouseMove\n");
	}
	return FALSE;
}
/*
 *	绘制标题栏背景
 */
void CXSkinWnd::DrawWndTitleBarBk(CDC* pDC, RECT* pRect)
{
	CRect		rect(pRect);
	COLORREF	colors[] = COLOR_XSKIN_TITLEBARBK();

	ASSERT(sizeof(colors) / sizeof(COLORREF) >= 25);
	rect.MoveToXY(0, 0);
	for(int i = 2; i < 27; i++)
	{
		pDC->FillSolidRect(0, i, rect.Width(), 1, colors[i - 2]);
	}
}
/*
 *	绘制非客户区框架
 */
void CXSkinWnd::DrawWndFrame(CDC* pDC, RECT* pRect)
{
	CRect		rect(pRect);
	CPen		pen;
	CPen*		pOlPen;


	// 外框
	rect = *pRect;
	rect.MoveToXY(0, 0);
	pen.CreatePen(PS_SOLID, 2, COLOR_XSKIN_OUTERFRAME);
	pOlPen	= pDC->SelectObject(&pen);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);
	// 外圆角
	int		X = 10;
	int		Y = 14;
	rect.SetRect(0, 0, X, X);
	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, rect.left + rect.Width() / 2, rect.top, rect.left, rect.top + rect.Height() / 2);
	rect.SetRect(pRect->right - pRect->left - Y, 0, pRect->right - pRect->left, Y);
	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, rect.right, rect.top + rect.Width() / 2, rect.right - rect.Width() / 2, rect.top);
	pDC->SelectObject(pOlPen);
	pen.DeleteObject();

	// 内边框
	pen.CreatePen(PS_SOLID, 1, COLOR_XSKIN_INNERFRAME);
	pOlPen	= pDC->SelectObject(&pen);
	rect = *pRect;
	rect.MoveToXY(0, 0);
	rect.DeflateRect(1, 1, 2, 2);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->MoveTo(rect.right, rect.top + 26);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top + 25);
	// 内圆角
	// 	rect.SetRect(0, 0, 10, 10);
	// 	rect.MoveToXY(1, 1);
	// 	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, rect.left + rect.Width() / 2, rect.top, rect.left, rect.top + rect.Height() / 2);
	// 	rect.SetRect(pRect->right - pRect->left - Y - 1, 1, pRect->right - pRect->left - 1, 1 + Y);
	// 	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, rect.right, rect.top + rect.Width() / 2, rect.left + rect.Height() / 2, rect.top);
	pDC->SelectObject(pOlPen);
	pen.DeleteObject();
}