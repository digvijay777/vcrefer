/************************************************************************/
/*                                                                      */
/*                       窗体选择按钮                                   */
/*                                                                      */
/*                                                                      */
/*  说明:当在按钮上按下鼠标左键时选择开始，此时拖动鼠标到要选择窗体上   */
/*       然后松开左键，选择完成.                                        */
/*  创作时间:2009年11月17日                                             */
/************************************************************************/

#include "stdafx.h"
#include "SelectWndBtn.h"
#include ".\selectwndbtn.h"

#define DEFAULT_REVERTWIDTH		3			// 默认选中窗体反色边框

IMPLEMENT_DYNAMIC(CSelectWndBtn, CButton)
CSelectWndBtn::CSelectWndBtn()
{
	m_bLButtonDown = FALSE;
	m_hOldSelWnd = NULL;
	m_nRevertWidth = DEFAULT_REVERTWIDTH;
	m_hCursor = NULL;
	m_hOldCursor = NULL;
}

CSelectWndBtn::~CSelectWndBtn()
{
}


BEGIN_MESSAGE_MAP(CSelectWndBtn, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CSelectWndBtn 消息处理程序
/*
 *	鼠标按下时进行选择窗体
 *
 *  添加时间:2009-11-7
 */
void CSelectWndBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	EnableWindow(FALSE);
	SetCapture();
	m_bLButtonDown = TRUE;
	m_hOldSelWnd = NULL;
	// 通知父窗体开始拖动
	NotifyParent(UM_BEGINSELECTWND);
	// 改变鼠标样式
	if(NULL == m_hCursor)
		return;
	m_hOldCursor = ::GetCursor();
	::SetCursor(m_hCursor);
//	CButton::OnLButtonDown(nFlags, point);
}
/*
 *	鼠标弹起时选择窗体
 *
 *  添加时间:2009-11-7
 */
void CSelectWndBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(FALSE == m_bLButtonDown)
	{
		CButton::OnLButtonUp(nFlags, point);
		return;
	}
	// 弹起动作
	POINT		pt					= {0};
	HWND		hWnd				= NULL;
	DWORD		dwWndProcessID		= 0;
	DWORD		dwCurProcessID		= 0;

	::GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	dwCurProcessID = ::GetCurrentProcessId();
	::GetWindowThreadProcessId(hWnd, &dwWndProcessID);
	ReleaseCapture();
	EnableWindow(TRUE);
	m_bLButtonDown = FALSE;
	// 添除边框
	InvertWindowFrame(m_hOldSelWnd, m_nRevertWidth);
	m_hOldSelWnd = NULL;
	// 是同一进程的窗体就不用绘制边框
	if(dwWndProcessID == dwCurProcessID)
	{
		hWnd = NULL;
	}
	// 通知父窗体结束拖动
	NotifyParent(UM_ENDSELECTWND, (WPARAM)hWnd);
	// 还原鼠标样式
	if(NULL == m_hCursor)
		return;
	::SetCursor(m_hOldCursor);
	return;
}
/*
 *	鼠标移动时高量窗体
 *
 *  添加时间:2009-11-8
 */
void CSelectWndBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	CButton::OnMouseMove(nFlags, point);

	// 画窗体边框
	DWORD		dwWndProcessID		= 0;
	DWORD		dwCurProcessID		= 0;
	HWND		hWnd				= NULL;
	POINT		pt					= {0};

	// 判断是否同一窗体
	::GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	if(hWnd == m_hOldSelWnd)
	{
		// 已经选择的窗体就不需要重绘
		return ;
	}
	// 反色旧窗体边框
	InvertWindowFrame(m_hOldSelWnd, m_nRevertWidth);
	// 判断是否同一进程
	dwCurProcessID = ::GetCurrentProcessId();
	::GetWindowThreadProcessId(hWnd, &dwWndProcessID);
	// 是同一进程的窗体就不用绘制边框
	if(dwWndProcessID == dwCurProcessID)
	{
		m_hOldSelWnd = NULL;
		NotifyParent(UM_CHANGESELECTWND);
		return;
	}
	// 反色新窗体边框
	InvertWindowFrame(hWnd, m_nRevertWidth);
	NotifyParent(UM_CHANGESELECTWND, (WPARAM)hWnd);
	m_hOldSelWnd = hWnd;
}

/*
 *	反色矩形区域边框
 *
 *  添加时间:2009-11-8
 */
void CSelectWndBtn::InvertRectangle(HDC hDC, LPRECT lpRect, int nWidth)
{
	RECT		inverRect;

	inverRect = *lpRect;
	inverRect.bottom = inverRect.top + nWidth;
	inverRect.right -= nWidth;
	::InvertRect(hDC, &inverRect);
	inverRect = *lpRect;
	inverRect.left = inverRect.right - nWidth;
	inverRect.bottom -= nWidth;
	::InvertRect(hDC, &inverRect);
	inverRect = *lpRect;
	inverRect.top = inverRect.bottom - nWidth;
	inverRect.left += nWidth;
	::InvertRect(hDC, &inverRect);
	inverRect = *lpRect;
	inverRect.right = inverRect.left + nWidth;
	inverRect.top += nWidth;
	::InvertRect(hDC, &inverRect);
}
/*
 *	反色窗体边框
 *
 *  添加时间:2009-11-8
 */
void CSelectWndBtn::InvertWindowFrame(HWND hWnd, int nWidth)
{
	if(!IsWindow(hWnd))
		return;

	CWnd*			pWnd		= CWnd::FromHandle(hWnd);
	CRect			rect;
	CWindowDC		dc(pWnd);

	pWnd->GetWindowRect(&rect);
	rect.MoveToXY(0, 0);

	InvertRectangle(dc.m_hDC, &rect, nWidth);
}
/*
 *	通知宿主消息
 *
 *  添加时间:2009-11-8
 */
void CSelectWndBtn::NotifyParent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::PostMessage(::GetParent(m_hWnd), msg, wParam, lParam);
}
/*
 *	设置鼠标样式
 *
 *  添加时间:2009-11-8
 */
void CSelectWndBtn::SetCursor(UINT nCursorID)
{
	m_hCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(nCursorID));
}