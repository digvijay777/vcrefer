// StatusSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "StatusSplitter.h"
#include ".\statussplitter.h"

enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // just for keyboard
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};
// CStatusSplitter

IMPLEMENT_DYNAMIC(CStatusHSplitter, CSplitterWnd)
CStatusHSplitter::CStatusHSplitter()
{
// 	m_cxSplitter = 15;    //must >=4 ,拖动时横向拖动条的宽度
	m_cySplitter = 6;     //must >=4 ,拖动时竖向拖动条的宽度
// 	m_cxBorderShare = 0; //按下鼠标时横向拖动条的偏移量
// 	m_cyBorderShare = 10; //按下鼠标时竖向拖动条的偏移量
// 	m_cxSplitterGap= 5; //静止时横向拖动条的宽度  
	m_cySplitterGap = m_cySplitter = 6; //静止时横向拖动条的宽度
	m_bSplitter = TRUE;
}

CStatusHSplitter::~CStatusHSplitter()
{
}


BEGIN_MESSAGE_MAP(CStatusHSplitter, CSplitterWnd)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

// 参数为 CRuntimeClass* pViewClass
BOOL CStatusHSplitter::InitSplitter(CWnd* pParent, CCreateContext* pContext, int nRows, ...)
{
	BOOL		bRes		= FALSE;
	va_list		vaView;
	int			i			= 0;

	// 创建初图
	bRes = CreateStatic(pParent, nRows, 1);
	if(FALSE == bRes) 
		return FALSE;
	va_start(vaView, nRows);
	for(i = 0; i < nRows; i++)
	{
		CRuntimeClass*	pViewClass	= va_arg(vaView, CRuntimeClass*);

		bRes = CreateView(i, 0, pViewClass, CSize(0, 0), pContext);
		if(FALSE == bRes)
			break;
	}
	va_end(vaView);
	if(i < nRows)
	{
		ASSERT(i < nRows);
		return FALSE;
	}
	// 初始大小
	InitRect(nRows);

	return TRUE;
}

// 初始化区域
void CStatusHSplitter::InitRect(int nRows)
{
	RECT		rect		= {0};

	GetParent()->GetClientRect(&rect);
	SetRowInfo(0, rect.bottom - rect.top - (nRows - 1) * 100, 50);
	for(int i = 1; i < nRows; i++)
	{
		SetRowInfo(i, 50, 0);
	}
}

void CStatusHSplitter::OnInvertTracker(const CRect& rect)
{
	CRect		rt		= rect;

	rt.bottom = rect.top + 6;
	CSplitterWnd::OnInvertTracker(rt);
}

void CStatusHSplitter::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
}

void CStatusHSplitter::StartTracking(int ht)
{
	CSplitterWnd::StartTracking(ht);
	// 重设限制大小
	if(m_bTracking && (ht >= vSplitterBar1 && ht <= vSplitterBar15))
	{
		m_rectLimit.bottom = 0;
		for(int i = 0; i <= ht - vSplitterBar1 + 1; i++)
		{
			int			nCur		= 0;
			int			nMin		= 0;

			GetRowInfo(i, nCur, nMin);
			m_rectLimit.bottom += nCur;
		}
	}
}

void CStatusHSplitter::StopTracking(BOOL bAccept)
{
	CSplitterWnd::StopTracking(bAccept);
}

void CStatusHSplitter::TrackRowSize(int y, int row)
{
// 	CSplitterWnd::TrackRowSize(y, row);
// 	return;
	if(row < (m_nMaxRows-2))
	{
		int		nCur		= 0;
		int		nMin		= 0;
		int		nY			= 0;

		GetRowInfo(row, nCur, nMin);
		nY += nCur;
		GetRowInfo(row + 1, nCur, nMin);
		nY += nCur;

		if(m_pRowInfo[row].nMinSize > y)
			y = 0;
		if(m_pRowInfo[row+1].nMinSize > (nY - y))
			y = nY;

		m_pRowInfo[row].nIdealSize = y;
		m_pRowInfo[row+1].nIdealSize = nY - y;	

		return;
	}

	CSplitterWnd::TrackRowSize(y, row);
	
//	CPoint		pt(0, y);

// 	ClientToScreen(&pt);
// 	GetPane(row, 0)->ScreenToClient(&pt);
// 	m_pRowInfo[row].nIdealSize = pt.y;
// 	pt.SetPoint(0, nY + m_cySplitterGap);
// 	ClientToScreen(&pt);
// 	GetPane(row+1, 0)->ScreenToClient(&pt);
// 	m_pRowInfo[row+1].nIdealSize = pt.y;
// 	pt.SetPoint(0, nY-y);
// 	ClientToScreen(&pt);
// 	GetPane(row+1, 0)->ScreenToClient(&pt);
// 	m_pRowInfo[row+1].nIdealSize -= pt.y;
// 	CSplitterWnd::TrackRowSize(y, row);
// 
// 	CPoint pt(0, y);
// 	ClientToScreen(&pt);
// 	GetPane(row, 0)->ScreenToClient(&pt);
// 	m_pRowInfo[row].nIdealSize = pt.y;      // new size
// 	if (pt.y < m_pRowInfo[row].nMinSize)
// 	{
// 		// resized too small
// 		m_pRowInfo[row].nIdealSize = 0; // make it go away
// 		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
// 			DeleteRow(row);
// 	}
// 	else if (m_pRowInfo[row].nCurSize + m_pRowInfo[row+1].nCurSize
// 		< pt.y + m_pRowInfo[row+1].nMinSize)
// 	{
// 		// not enough room for other pane
// 		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
// 			DeleteRow(row + 1);
// 	}
}

void CStatusHSplitter::OnSize(UINT nType, int cx, int cy)
{
	//RecalcLayout();
	CSplitterWnd::OnSize(nType, cx, cy);
	return;
	if(NULL != m_pRowInfo)
	{
		int			Y		= 0;
		CRect		rect;

		GetClientRect(&rect);
		Y = rect.Height() - (m_nMaxRows - 1) * m_cySplitterGap;
		for(int i = m_nMaxRows - 1; i > 0; i--)
		{
			int		nCur		= 0;
			int		nMin		= 0;

			GetRowInfo(i, nCur, nMin);
			Y -= nCur;
		}
		m_pRowInfo[0].nIdealSize = Y;
		RecalcLayout();
	}

	CWnd::OnSize(nType, cx, cy);

}

void CStatusHSplitter::RecalcLayout()
{
	if(NULL == m_pRowInfo || NULL == m_pColInfo)
	{
		return;
	}

	CSplitterWnd::RecalcLayout();
	return;
	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);

	CRect rectInside;
	GetInsideRect(rectInside);

	// 计算大小
	int		nSizeY		= rectInside.Height() - (m_nRows - 1) * m_cySplitterGap;
	for (int row = m_nRows - 1; row > 0; row--)
	{
		int		y = (0 > m_pRowInfo[row].nIdealSize)?m_pRowInfo[row].nCurSize:m_pRowInfo[row].nIdealSize;

		if(y < m_pRowInfo[row].nMinSize)
			y = 0;
		if((nSizeY - y) < 0)
			y = nSizeY;
		nSizeY -= y;
		if(nSizeY < 0)
			nSizeY = 0;

		m_pRowInfo[row].nCurSize = 0;
		//m_pRowInfo[row].nIdealSize = y/*(0==row)?nSizeY:y*/;
// 		m_pRowInfo[row].nCurSize = y;
		m_pRowInfo[row].nIdealSize = y;
	}
	m_pRowInfo[0].nCurSize = 0;
	m_pRowInfo[0].nIdealSize = nSizeY;
	CSplitterWnd::RecalcLayout();
	return;

	HDWP		hDwp		= NULL;

	hDwp = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);
	int x = rectClient.left;
	int cx = rectInside.Width();
	int y = rectClient.top;
	for (int row = 0; row < m_nRows; row++)
	{
		int cy = m_pRowInfo[row].nCurSize;
		CWnd* pWnd = GetPane(row, 0);
		DeferWindowPos(hDwp, pWnd->GetSafeHwnd(), NULL, x, y, cx, cy, SWP_NOACTIVATE);
		y += cy + m_cySplitterGap;
	}

	::EndDeferWindowPos(hDwp);

	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}
void CStatusHSplitter::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
// 	CSplitterWnd::OnWindowPosChanging(lpwndpos);

	CRect		Rect;

	GetWindowRect(Rect);
	if(Rect.Height() == lpwndpos->cy)
	{
		CSplitterWnd::OnWindowPosChanging(lpwndpos);
		return;
	}

	if((*lpwndpos).cy == 0)
	{
		CSplitterWnd::OnWindowPosChanging(lpwndpos);
		return;
	}

	int			nCy;
	int			nCyMin;

	GetRowInfo(0, nCy, nCyMin);
	nCy = nCy - Rect.Height() + lpwndpos->cy;
	if(nCy < 0)
		nCy = 0;
	SetRowInfo(0, nCy, nCyMin);

	CSplitterWnd::OnWindowPosChanging(lpwndpos);

}
