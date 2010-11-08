// BarView.cpp : implementation file
//

#include "stdafx.h"
#include "SplitterTest.h"
#include "BarView.h"
#include ".\barview.h"

#define DEF_BARVIEW_TITLE			18
#define DEF_BARVIEW_HEIGTH			100
#define DEF_BARVIEW_MAXHEIGHT		500
#define DEF_BARVIEW_TRACKING		3
// CBarView

IMPLEMENT_DYNCREATE(CBarView, CView)

CBarView::CBarView()
{
	m_pMainView = 0;
	m_hDefCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	m_hSizeCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));	
	m_bTracking = FALSE;
	m_nTrackerBar = -1;
}

CBarView::~CBarView()
{

}

BEGIN_MESSAGE_MAP(CBarView, CView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CBarView drawing

void CBarView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	CRect		rect;

	GetClientRect(&rect);
	for(int i = m_vctBar.size() - 1; i >= 0; i--)
	{
		CRect	rtBar		= rect;

		rtBar.top = rtBar.bottom - DEF_BARVIEW_TITLE - ((FALSE == m_vctBar[i].bExpend)?0:m_vctBar[i].nHeight);
		rtBar.bottom = rtBar.top + DEF_BARVIEW_TITLE;
		rect.bottom = rtBar.top;
		DrawBarTitle(pDC, rtBar, i);
	}
}


// CBarView diagnostics

#ifdef _DEBUG
void CBarView::AssertValid() const
{
	CView::AssertValid();
}

void CBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

BOOL CBarView::SetView(CRuntimeClass* pViewClass)
{
	ASSERT(NULL == m_pMainView);

	if(NULL != m_pMainView)
		return FALSE;
	m_pMainView = (CWnd *)pViewClass->CreateObject();
	if(NULL == m_pMainView)
		return FALSE;

	m_pMainView->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_MAINVIEW);
	return TRUE;
}

int CBarView::AddBarView(CRuntimeClass* pViewClass, LPCSTR lpTitle /* = NULL */)
{
	BARVIEWINFO			info;
	CWnd*				pWnd;

	info.bExpend = FALSE;
	info.nHeight = DEF_BARVIEW_HEIGTH;
	pWnd = (CWnd *)pViewClass->CreateObject();
	if(NULL == pWnd)
	{
		ASSERT(NULL != pWnd);
		return -1;
	}
	pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_MAINVIEW);
	info.pWnd = pWnd;
	info.strTtile = (NULL != lpTitle)?lpTitle:"";
	info.nMaxHeight = DEF_BARVIEW_MAXHEIGHT;
	
	m_vctBar.push_back(info);

	return m_vctBar.size() - 1;
}

void CBarView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	RecalcLayout();
}

void CBarView::RecalcLayout()
{
	CRect		rect;
	HDWP		hDwp;

	GetClientRect(&rect);
	hDwp = BeginDeferWindowPos(m_vctBar.size() + 1);

	for(int i = m_vctBar.size() - 1; i >= 0; i--)
	{
		CRect	rtBar		= rect;

		rtBar.top = rtBar.bottom - DEF_BARVIEW_TITLE - ((FALSE == m_vctBar[i].bExpend)?0:m_vctBar[i].nHeight);
		rect.bottom = rtBar.top;
		rtBar.top += DEF_BARVIEW_TITLE;
		if(NULL != m_vctBar[i].pWnd && IsWindow(m_vctBar[i].pWnd->GetSafeHwnd()))
		{
			if(NULL != hDwp)
				DeferWindowPos(hDwp, m_vctBar[i].pWnd->GetSafeHwnd(), NULL
					, rtBar.left, rtBar.top, rtBar.Width(), rtBar.Height(), SWP_NOACTIVATE);
			else
				m_vctBar[i].pWnd->MoveWindow(&rtBar, TRUE);
		}
	}

	if(NULL != m_pMainView && IsWindow(m_pMainView->GetSafeHwnd()))
	{
		if(NULL != hDwp)
			DeferWindowPos(hDwp, m_pMainView->GetSafeHwnd(), NULL
				, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
		else
			m_pMainView->MoveWindow(&rect, TRUE);
	}
	EndDeferWindowPos(hDwp);
}

void CBarView::DrawBarTitle(CDC* pDC, CRect& rect, int nNO)
{
	pDC->DrawText(m_vctBar[nNO].strTtile.c_str(), m_vctBar[nNO].strTtile.size(), &rect, DT_VCENTER | DT_SINGLELINE);
}

void CBarView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int		ht		= HitTest(point);

	if(ht >= 0 && ht < m_vctBar.size())
	{
		m_vctBar[ht].bExpend = !m_vctBar[ht].bExpend;
		RecalcLayout();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

int CBarView::HitTest(CPoint& pt)
{
	CRect		rect;

	GetClientRect(&rect);
	for(int i = m_vctBar.size() - 1; i >= 0; i--)
	{
		CRect	rtBar		= rect;

		rtBar.top = rtBar.bottom - DEF_BARVIEW_TITLE - ((FALSE == m_vctBar[i].bExpend)?0:m_vctBar[i].nHeight);
		rtBar.bottom = rtBar.top + DEF_BARVIEW_TITLE;
		rect.bottom = rtBar.top;
		if(rtBar.PtInRect(pt))
			return i;
	}

	return -1;
}

int CBarView::HitSpliter(CPoint& pt)
{
	CRect		rect;

	GetClientRect(&rect);
	for(int i = m_vctBar.size() - 1; i >= 0; i--)
	{
		CRect	rtBar		= rect;

		rtBar.top = rtBar.bottom - DEF_BARVIEW_TITLE - ((FALSE == m_vctBar[i].bExpend)?0:m_vctBar[i].nHeight);
		rtBar.bottom = rtBar.top + DEF_BARVIEW_TRACKING;
		rtBar.DeflateRect(1, 0, 1, 0);
		rect.bottom = rtBar.top;
		if(rtBar.PtInRect(pt))
			return i;
	}
}

void CBarView::OnMouseMove(UINT nFlags, CPoint point)
{
	int			ht		= HitSpliter(point);

	if(ht >= 0 && ht < m_vctBar.size() && m_vctBar[ht].bExpend)
	{
		if(GetCursor() != m_hSizeCursor)
		{
			SetCursor(m_hSizeCursor);
		}
		return ;
	}

	if(GetCapture() != this)
		StopTracking(FALSE);

	if(m_bTracking)
	{
		CRect		rect;

		GetClientRect(&rect);
		//TRACE("OnMouseMove %d,%d\r\n", point.x, point.y);
		//ScreenToClient(&point);
		rect.DeflateRect(1, 0, 1, 0);
		rect.top = point.y;
		if(rect.top < m_rectLimt.top)
			rect.top = m_rectLimt.top;
		else if(rect.top > (m_rectLimt.bottom - DEF_BARVIEW_TRACKING - DEF_BARVIEW_TITLE))
			rect.top = m_rectLimt.bottom - DEF_BARVIEW_TRACKING - DEF_BARVIEW_TITLE;
		rect.bottom = rect.top + DEF_BARVIEW_TRACKING;
		if(m_rectTracker == rect)
			return;
		if(!m_rectTracker.IsRectEmpty())
			OnInvertTracker(m_rectTracker);
		m_rectTracker = rect;
		TRACE("OnMouseMove (%d, %d), (%d, %d), (%d, %d)\n"
			, point.x, point.y
			, m_rectTracker.top, m_rectTracker.bottom
			, m_rectLimt.top, m_rectLimt.bottom);
		OnInvertTracker(m_rectTracker);
		return;
	}

	if(GetCursor() != m_hDefCursor)
	{
		SetCursor(m_hDefCursor);
	}

	CView::OnMouseMove(nFlags, point);
}

void CBarView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bTracking)
		return;

	StartTracking(HitSpliter(point));
}

BOOL CBarView::GetBarViewRect(int nNO, CRect& rect)
{
	if(nNO < 0 || nNO >= m_vctBar.size())
		return FALSE;

	GetClientRect(rect);
	for(int i = m_vctBar.size() - 1; i > nNO; i--)
	{
		rect.bottom -= DEF_BARVIEW_TITLE;
		rect.bottom -= ((FALSE != m_vctBar[i].bExpend)?m_vctBar[i].nHeight:0);
	}

	rect.top = rect.bottom;
	rect.top -= DEF_BARVIEW_TITLE;
	rect.top -= ((FALSE != m_vctBar[nNO].bExpend)?m_vctBar[nNO].nHeight:0);
}

void CBarView::OnLButtonUp(UINT nFlags, CPoint point)
{
	StopTracking(TRUE);
}

void CBarView::StartTracking(int nNO)
{
	if(nNO < 0 || nNO >= m_vctBar.size())
		return;

	if(!m_vctBar[nNO].bExpend)
		return;	// 关闭时不永允退出

	SetCapture();
	m_bTracking = TRUE;
	SetFocus();
	
	m_rectLimt.SetRectEmpty();
	m_rectTracker.SetRectEmpty();
	GetBarViewRect(nNO, m_rectLimt);
	m_rectTracker = m_rectLimt;
	m_rectLimt.top = m_rectLimt.bottom - m_vctBar[nNO].nMaxHeight - DEF_BARVIEW_TITLE;
	if(m_rectLimt.top < 0)
		m_rectLimt.top = 0;
	m_rectTracker.bottom = m_rectTracker.top + DEF_BARVIEW_TRACKING;
	OnInvertTracker(m_rectTracker);
	SetCursor(m_hSizeCursor);
	m_nTrackerBar = nNO;
}

void CBarView::StopTracking(BOOL bActive)
{
	if(!m_bTracking)
		return;

	ReleaseCapture();
	m_bTracking = FALSE;

	if(!bActive)
		return;

	if(!m_rectTracker.IsRectEmpty())
		OnInvertTracker(m_rectTracker);

	int		nNewHeight		= m_rectLimt.bottom - m_rectTracker.bottom; 

	m_rectTracker.SetRectEmpty();
	if(0 > m_nTrackerBar || m_nTrackerBar >= m_vctBar.size())
		return;
	nNewHeight -= DEF_BARVIEW_TITLE;
	if(nNewHeight < 0)
		nNewHeight = 0;
	if(nNewHeight > m_vctBar[m_nTrackerBar].nMaxHeight)
		nNewHeight = m_vctBar[m_nTrackerBar].nMaxHeight;

	m_vctBar[m_nTrackerBar].nHeight = nNewHeight;
	RecalcLayout();
}

void CBarView::OnInvertTracker(CRect rtTracker)
{
	CDC* pDC = GetDC();
	// invert the brush pattern (looks just like frame window sizing)
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	pDC->PatBlt(rtTracker.left, rtTracker.top, rtTracker.Width(), rtTracker.Height(), PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);
}
