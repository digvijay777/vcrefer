/************************************************************************/
/*                                                                      */
/*                       ����ѡ��ť                                   */
/*                                                                      */
/*                                                                      */
/*  ˵��:���ڰ�ť�ϰ���������ʱѡ��ʼ����ʱ�϶���굽Ҫѡ������   */
/*       Ȼ���ɿ������ѡ�����.                                        */
/*  ����ʱ��:2009��11��17��                                             */
/************************************************************************/

#include "stdafx.h"
#include "SelectWndBtn.h"
#include ".\selectwndbtn.h"

#define DEFAULT_REVERTWIDTH		3			// Ĭ��ѡ�д��巴ɫ�߿�

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



// CSelectWndBtn ��Ϣ�������
/*
 *	��갴��ʱ����ѡ����
 *
 *  ���ʱ��:2009-11-7
 */
void CSelectWndBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	EnableWindow(FALSE);
	SetCapture();
	m_bLButtonDown = TRUE;
	m_hOldSelWnd = NULL;
	// ֪ͨ�����忪ʼ�϶�
	NotifyParent(UM_BEGINSELECTWND);
	// �ı������ʽ
	if(NULL == m_hCursor)
		return;
	m_hOldCursor = ::GetCursor();
	::SetCursor(m_hCursor);
//	CButton::OnLButtonDown(nFlags, point);
}
/*
 *	��굯��ʱѡ����
 *
 *  ���ʱ��:2009-11-7
 */
void CSelectWndBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(FALSE == m_bLButtonDown)
	{
		CButton::OnLButtonUp(nFlags, point);
		return;
	}
	// ������
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
	// ����߿�
	InvertWindowFrame(m_hOldSelWnd, m_nRevertWidth);
	m_hOldSelWnd = NULL;
	// ��ͬһ���̵Ĵ���Ͳ��û��Ʊ߿�
	if(dwWndProcessID == dwCurProcessID)
	{
		hWnd = NULL;
	}
	// ֪ͨ����������϶�
	NotifyParent(UM_ENDSELECTWND, (WPARAM)hWnd);
	// ��ԭ�����ʽ
	if(NULL == m_hCursor)
		return;
	::SetCursor(m_hOldCursor);
	return;
}
/*
 *	����ƶ�ʱ��������
 *
 *  ���ʱ��:2009-11-8
 */
void CSelectWndBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	CButton::OnMouseMove(nFlags, point);

	// ������߿�
	DWORD		dwWndProcessID		= 0;
	DWORD		dwCurProcessID		= 0;
	HWND		hWnd				= NULL;
	POINT		pt					= {0};

	// �ж��Ƿ�ͬһ����
	::GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	if(hWnd == m_hOldSelWnd)
	{
		// �Ѿ�ѡ��Ĵ���Ͳ���Ҫ�ػ�
		return ;
	}
	// ��ɫ�ɴ���߿�
	InvertWindowFrame(m_hOldSelWnd, m_nRevertWidth);
	// �ж��Ƿ�ͬһ����
	dwCurProcessID = ::GetCurrentProcessId();
	::GetWindowThreadProcessId(hWnd, &dwWndProcessID);
	// ��ͬһ���̵Ĵ���Ͳ��û��Ʊ߿�
	if(dwWndProcessID == dwCurProcessID)
	{
		m_hOldSelWnd = NULL;
		NotifyParent(UM_CHANGESELECTWND);
		return;
	}
	// ��ɫ�´���߿�
	InvertWindowFrame(hWnd, m_nRevertWidth);
	NotifyParent(UM_CHANGESELECTWND, (WPARAM)hWnd);
	m_hOldSelWnd = hWnd;
}

/*
 *	��ɫ��������߿�
 *
 *  ���ʱ��:2009-11-8
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
 *	��ɫ����߿�
 *
 *  ���ʱ��:2009-11-8
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
 *	֪ͨ������Ϣ
 *
 *  ���ʱ��:2009-11-8
 */
void CSelectWndBtn::NotifyParent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::PostMessage(::GetParent(m_hWnd), msg, wParam, lParam);
}
/*
 *	���������ʽ
 *
 *  ���ʱ��:2009-11-8
 */
void CSelectWndBtn::SetCursor(UINT nCursorID)
{
	m_hCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(nCursorID));
}