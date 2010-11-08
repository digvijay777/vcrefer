// ImgTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImgTabCtrl.h"
#include ".\imgtabctrl.h"
#include "MemDc.h"

// CImgTabCtrl

IMPLEMENT_DYNAMIC(CImgTabCtrl, CTabCtrl)
CImgTabCtrl::CImgTabCtrl()
{
	m_colText = RGB(0xCC, 0xCC, 0xCC);
	m_colSelText = RGB(0x0, 0x0, 0x0);
	memset(&m_ImgInfoItem, 0, sizeof(m_ImgInfoItem));
	memset(&m_ImgInfoIcon, 0, sizeof(m_ImgInfoIcon));
	m_bTrackMouseEvent = FALSE;
	m_nMouseHover = -1;
	m_bHasMouseHoverEvent = FALSE;
	m_nIconLeftPos = 8;
	m_nIconRightPos = 5;
}

CImgTabCtrl::~CImgTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CImgTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CImgTabCtrl::PreSubclassWindow()
{
	InitTabCtrl();

	CTabCtrl::PreSubclassWindow();
}

// 初始化TAB控件
void CImgTabCtrl::InitTabCtrl()
{
	DWORD		dwAddStyle		= TCS_BUTTONS | TCS_FOCUSNEVER | TCS_OWNERDRAWFIXED | TCS_SINGLELINE | TCS_FIXEDWIDTH;
	DWORD		dwDelStyle		= TCS_FOCUSONBUTTONDOWN | TCS_MULTILINE | TCS_MULTISELECT;

	ModifyStyle(dwDelStyle, dwAddStyle);
	
	LOGFONT			logFont		= {0};

	GetFont()->GetLogFont(&logFont);
	logFont.lfWeight = 700;
	if(NULL != m_FontSel.GetSafeHandle())
		m_FontSel.DeleteObject();
	m_FontSel.CreateFontIndirect(&logFont);
}
void CImgTabCtrl::SetTabVertical(BOOL bVertical /* = FALSE */)
{
	DWORD			dwAddStyle		= TCS_VERTICAL;
	CSize			size;

	if(FALSE == bVertical)
	{
		ModifyStyle(dwAddStyle, 0);
		size.cx = m_ImgInfoItem.rcImage.right - m_ImgInfoItem.rcImage.left;
		size.cy = m_ImgInfoItem.rcImage.bottom - m_ImgInfoItem.rcImage.top;
	}
	else
	{
		ModifyStyle(0, dwAddStyle);
		size.cx = m_ImgInfoItem.rcImage.bottom - m_ImgInfoItem.rcImage.top;
		size.cy = m_ImgInfoItem.rcImage.right - m_ImgInfoItem.rcImage.left;
	}

	SetItemSize(size);
}

BOOL CImgTabCtrl::SetIconPos(int nLeft /* = 8 */, int nRight /* = 5 */)
{
	m_nIconLeftPos = nLeft;
	m_nIconRightPos = nRight;
	return TRUE;
}

void CImgTabCtrl::SetTextColor(COLORREF colText, COLORREF colSelText)
{
	m_colText = colText;
	m_colSelText = colSelText;
}

BOOL CImgTabCtrl::LoadImages(UINT uImgItem, int nState /* = 2 */, 
							 UINT uImgIcon /* = -1 */, COLORREF colBk /* = RGB */)
{
	CBitmap			bmpItem;
	CBitmap			bmpIcon;

	if(FALSE == bmpItem.LoadBitmap(uImgItem))
		return FALSE;
	if(-1 != uImgIcon && FALSE != bmpIcon.LoadBitmap(uImgIcon))
	{
		return SetBitmaps(&bmpItem, nState, &bmpIcon, colBk);
	}

	return SetBitmaps(&bmpItem, nState, NULL, colBk);
}

BOOL CImgTabCtrl::LoadImages(LPSTR lpImgItem, int nState /* = 2 */, 
							 LPSTR lpImgIcon /* = -1 */, COLORREF colBk /* = RGB */)
{
	CBitmap			bmpItem;
	CBitmap			bmpIcon;

	if(FALSE == CMemDC::LoadBitmapFromFile(&bmpItem, lpImgItem))
		return FALSE;
	if(NULL != lpImgIcon && FALSE != CMemDC::LoadBitmapFromFile(&bmpIcon, lpImgIcon))
	{
		return SetBitmaps(&bmpItem, nState, &bmpIcon, colBk);
	}

	return SetBitmaps(&bmpItem, nState, NULL, colBk);
}

BOOL CImgTabCtrl::SetBitmaps(CBitmap* pBmpItem, int nState /* = 2 */, 
							 CBitmap* pBmpIcon /* = NULL */, COLORREF colBk /* = RGB */)
{
	if(NULL == pBmpItem || NULL == pBmpItem->GetSafeHandle())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(2 == nState || 4 == nState);
	if(NULL != m_ImgList.GetSafeHandle())
		m_ImgList.DeleteImageList();

	BITMAP		bmp;

	pBmpItem->GetBitmap(&bmp);
	m_ImgList.Create(bmp.bmWidth / nState, bmp.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_ImgList.Add(pBmpItem, colBk);
	memset(&m_ImgInfoItem, 0, sizeof(m_ImgInfoItem));
	m_ImgList.GetImageInfo(0, &m_ImgInfoItem);

	// 改变TAB的行高 
	CSize			size;
	size.cx = m_ImgInfoItem.rcImage.right - m_ImgInfoItem.rcImage.left;
	size.cy = m_ImgInfoItem.rcImage.bottom - m_ImgInfoItem.rcImage.top;
	SetItemSize(size);

	m_bHasMouseHoverEvent = (4 == nState);

	if(NULL != m_ImgIcon.GetSafeHandle())
		m_ImgIcon.DeleteImageList();
	memset(&m_ImgInfoIcon, 0, sizeof(m_ImgInfoIcon));

	if(NULL == pBmpIcon)
		return TRUE;
	// 设置ICON
	pBmpIcon->GetBitmap(&bmp);
	m_ImgIcon.Create(bmp.bmHeight, bmp.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_ImgIcon.Add(pBmpIcon, colBk);
	m_ImgIcon.GetImageInfo(0, &m_ImgInfoIcon);

	return TRUE;
}
// 绘制控件
void CImgTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

}
// 绘制项
void CImgTabCtrl::DrawItem_Bk(CDC* pDC, CRect& rect, int nState)
{
	int			nImage		= 0;

	if(ODS_SELECTED & nState)
		nImage = (m_bHasMouseHoverEvent)?2:1;
	else if(ODS_INACTIVE & nState)
		nImage = (m_bHasMouseHoverEvent)?1:0;

	m_ImgList.Draw(pDC, nImage, rect.TopLeft(), ILD_TRANSPARENT);
}
// 绘制文本
void CImgTabCtrl::DrawItem_Text(CDC* pDC, int nItem, LPCSTR lpText, CRect& rect, int nState)
{
	CRect			rtWnd		= rect;
	CSize			size		= pDC->GetTextExtent(lpText);
	CFont*			pFont		= NULL;
	
	if(NULL != m_ImgIcon.GetSafeHandle())
	{
		rtWnd.left += m_nIconLeftPos + (m_ImgInfoIcon.rcImage.right - m_ImgInfoIcon.rcImage.left) + m_nIconRightPos;
	}
	else
	{
		rtWnd.left += (rtWnd.Width() - size.cx) / 2;
	}

	pDC->SetBkMode(TRANSPARENT);
	if(nState & ODS_SELECTED)
	{
		pDC->SetTextColor(m_colSelText);
		pFont = pDC->SelectObject(&m_FontSel);
	}
	else
	{
		pDC->SetTextColor(m_colText);
		pFont = pDC->SelectObject(GetFont());
	}
	pDC->DrawText(lpText, &rtWnd, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	pDC->SelectObject(pFont);
}
// 绘制图标
void CImgTabCtrl::DrawItem_Icon(CDC* pDC, int nImage, CRect& rect)
{
	if(NULL == m_ImgIcon.GetSafeHandle())
		return;

	POINT			point;
	CRect			rtImage(m_ImgInfoIcon.rcImage);

	point.y = rect.top + (rect.Height() - rtImage.Height()) / 2;
	point.x = rect.left + m_nIconLeftPos;

	m_ImgIcon.Draw(pDC, nImage, point, ILD_TRANSPARENT);
}

BOOL CImgTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	CWnd		*pParent;
	CRect		WndRect;
	CRect		ParRect;
	POINT		OffPt;
	int			nSave;

	pParent = GetParent();

	GetWindowRect(&WndRect);
	pParent->GetWindowRect(&ParRect);

	OffPt.x = WndRect.left - ParRect.left;
	OffPt.y = WndRect.top - ParRect.top;

	nSave = pDC->SaveDC();

	pDC->SetWindowOrg(OffPt);
	pParent->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->m_hDC, 0);
	pDC->SetViewportOrg(OffPt);

	pDC->RestoreDC(nSave);
	return FALSE;

	return CTabCtrl::OnEraseBkgnd(pDC);
}

void CImgTabCtrl::OnPaint()
{
	CPaintDC	dc(this); // device context for painting
	CRect		rtClient;

	GetClientRect(&rtClient);
	CMemDC		memDC(&dc, rtClient);
	OnEraseBkgnd(&memDC);

	TCITEM					tcItem;
	CHAR					szText[2048];
	CRect					rect;

	for(int i = 0; i < GetItemCount(); i++)
	{
		memset(&tcItem, 0, sizeof(tcItem));
		memset(szText, 0, sizeof(szText));

		tcItem.mask = TCIF_IMAGE | TCIF_STATE | TCIF_TEXT;
		tcItem.dwStateMask = -1;
		tcItem.pszText = szText;
		tcItem.cchTextMax = sizeof(szText);
		GetItemRect(i, &rect);
		if(FALSE == GetItem(i, &tcItem))
			continue;
		// 绘制
		DrawItem_Bk(&memDC, rect, tcItem.dwState);
		DrawItem_Icon(&memDC, tcItem.iImage, rect);
		DrawItem_Text(&memDC, i, szText, rect, tcItem.dwState);
	}

}

void CImgTabCtrl::SetMouseHoverItem(int nItem)
{
	TCITEM		tcItem;
	RECT		rect;

	// 清除原来的
	for(int i = 0; i < GetItemCount(); i++)
	{
		memset(&tcItem, 0, sizeof(tcItem));
		tcItem.mask = TCIF_STATE;
		tcItem.dwStateMask = -1;
		GetItem(i, &tcItem);

		if((tcItem.dwState & ODS_SELECTED) && nItem == i)
			return;
		else if((tcItem.dwState & ODS_INACTIVE) && nItem == i)
			return;
		else if(tcItem.dwState & ODS_SELECTED)
			continue;

		if(tcItem.dwState & ODS_INACTIVE)
		{
			tcItem.dwState = 0;
			tcItem.dwStateMask = -1;
		}
		else if(nItem == i)
		{
			tcItem.dwState = ODS_INACTIVE;
			tcItem.dwStateMask = ODS_INACTIVE;
		}
		else
		{
			continue;
		}

		SetItem(i, &tcItem);
		GetItemRect(i, &rect);
		InvalidateRect(&rect);
	}
}
// 鼠标离开
LRESULT CImgTabCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTrackMouseEvent = FALSE;
	SetMouseHoverItem(-1);

	return 0;
}

void CImgTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(FALSE == m_bHasMouseHoverEvent)
	{
		CTabCtrl::OnMouseMove(nFlags, point);
		return;
	}

	if(FALSE == m_bTrackMouseEvent)
	{
		TRACKMOUSEEVENT			track		= {0};

		track.cbSize = sizeof(track);
		track.dwFlags = TME_LEAVE;
		track.hwndTrack = GetSafeHwnd();
		track.dwHoverTime = 1000;

		_TrackMouseEvent(&track);
	}

	TCHITTESTINFO		info		= {0};
	int					nItem		= -1;

	info.flags = TCHT_ONITEM;
	info.pt = point;
	nItem = HitTest(&info);
	SetMouseHoverItem(nItem);

	CTabCtrl::OnMouseMove(nFlags, point);
}
