// SafeRepairCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "SafeRepairCtrl.h"
#include ".\saferepairctrl.h"
#include <assert.h>
#include "../../../ExtClass/mfc/MemDc.h"

#define COLOR_ITEM_0		RGB(0xf0, 0xf8, 0xfd)
#define COLOR_ITEM_1		RGB(0xff, 0xff, 0xff)

#define MS_LBUTTONNORMAL		0
#define MS_LBUTTONDOWN			2
#define MS_LBUTTONHOVER			1
// CSafeRepairCtrl

IMPLEMENT_DYNAMIC(CSafeRepairCtrl, CListBox)
CSafeRepairCtrl::CSafeRepairCtrl()
{
	m_nItemHeight = 85;
	memset(&m_msMouse, 0, sizeof(m_msMouse));
	m_msMouse.nOldIndex = -1;
}

CSafeRepairCtrl::~CSafeRepairCtrl()
{
}


BEGIN_MESSAGE_MAP(CSafeRepairCtrl, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CSafeRepairCtrl message handlers


void CSafeRepairCtrl::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();

	DWORD		dwStyle		= GetWindowLong(m_hWnd, GWL_STYLE);

	assert(dwStyle & LBS_OWNERDRAWFIXED);
}


void CSafeRepairCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->itemID < 0 || lpDrawItemStruct->itemID >= m_vctSafeItem.size())
		return;

	DrawItem_ShowSafeItem(lpDrawItemStruct);
}

// 绘制项
void CSafeRepairCtrl::DrawItem_ShowSafeItem(LPDRAWITEMSTRUCT pItem)
{
	CRect		rtItem(pItem->rcItem);
	CDC*		pDC						= CDC::FromHandle(pItem->hDC);
	CMemDC		memDC(pDC, rtItem);
	SAFEDATA&	safeData				= m_vctSafeItem[pItem->itemID];
	CRect		rtDraw;
	
	rtItem.MoveToXY(0, 0);
	// 背景
	memDC.FillSolidRect(&rtItem, (pItem->itemID % 2 == 0)?COLOR_ITEM_0:COLOR_ITEM_1);
	
	// 绘制状态
	rtDraw = rtItem;
	rtDraw.left += 5;
	rtDraw.top = rtItem.Height() / 2 - m_sizeState.cy / 2;
	if(NULL != m_imgSafeState.GetSafeHandle())
	{
		m_imgSafeState.Draw(&memDC, safeData.bSafe, rtDraw.TopLeft(), ILD_TRANSPARENT);
	}

	// 绘制标题
	rtDraw.left += 5 + m_sizeState.cx;
	rtDraw.right = rtItem.right - 5 - m_sizeBtn.cx - 5;
	rtDraw.top = rtItem.top + 10;

	CFont			ftTitle;
	LOGFONT			logFont;
	CFont*			pOldFont		= NULL;
	CSize			sizeTitle;

	GetFont()->GetLogFont(&logFont);
	logFont.lfWeight = 700;
	ftTitle.CreateFontIndirect(&logFont);
	pOldFont = memDC.SelectObject(&ftTitle);
	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(safeData.strTitle, rtDraw, DT_SINGLELINE | DT_LEFT | DT_TOP);
	sizeTitle = pDC->GetTextExtent(safeData.strTitle);
	
	// 绘制说明
	rtDraw.top += sizeTitle.cy + 5;
	memDC.SelectObject(GetFont());
	memDC.DrawText(safeData.strDescrib, rtDraw, DT_WORDBREAK | DT_LEFT | DT_TOP);

	// 绘制按钮
	rtDraw = CalButtonRect(rtItem);
	if(NULL != m_imgBtn.GetSafeHandle())
	{
		m_imgBtn.Draw(&memDC, safeData.nBtnState, rtDraw.TopLeft(), ILD_TRANSPARENT);
		memDC.DrawText(safeData.strBtnText, &rtDraw, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	memDC.SelectObject(pOldFont);
}



void CSafeRepairCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	if(-1 != m_msMouse.nOldIndex)
	{
		m_vctSafeItem[m_msMouse.nOldIndex].nBtnState = MS_LBUTTONDOWN;
		m_msMouse.bDown = TRUE;
		SetCapture();
		Invalidate();
	}
	return;
}
void CSafeRepairCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(FALSE != m_msMouse.bDown)
	{
		ReleaseCapture();

		BOOL	bBOOL;
		int nIndex = ItemFromPoint(point, bBOOL);
		m_vctSafeItem[m_msMouse.nOldIndex].nBtnState = MS_LBUTTONNORMAL;
		if(nIndex == m_msMouse.nOldIndex)
		{
			CRect	rect;
			GetItemRect(nIndex,rect);
			CRect rectCheckIcon = CalButtonRect(rect);
			if(rectCheckIcon.PtInRect(point))
			{
				m_vctSafeItem[nIndex].nBtnState = MS_LBUTTONHOVER;
				GetParent()->PostMessage(UM_SAFEREPAIR_CLICKED, m_vctSafeItem[nIndex].nBtnID, 0);
				TRACE("PostMessage: UM_SAFEREPAIR_CLICKED %d\n", m_vctSafeItem[nIndex].nBtnID);
			}
		}

		Invalidate();
	}
	m_msMouse.bDown = FALSE;
	CListBox::OnLButtonUp(nFlags, point);
}

LRESULT CSafeRepairCtrl::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	CPoint			point(LOWORD(lParam), HIWORD(lParam));

	BOOL	bBOOL;
	int nIndex = ItemFromPoint(point, bBOOL);
	if(-1 == nIndex || m_vctSafeItem.size() <= 0)
		return 0;

	CRect	rect;
	GetItemRect(nIndex,rect);

	CRect rectCheckIcon = CalButtonRect(rect);

	if(rectCheckIcon.PtInRect(point))
	{
		if(-1 != m_msMouse.nOldIndex)
			m_vctSafeItem[m_msMouse.nOldIndex].nBtnState = MS_LBUTTONNORMAL;

		if(FALSE == m_msMouse.bDown)
			m_msMouse.nOldIndex = nIndex;

		if(FALSE == m_msMouse.bDown || nIndex == m_msMouse.nOldIndex)
			m_vctSafeItem[nIndex].nBtnState = (m_msMouse.bDown)?MS_LBUTTONDOWN:MS_LBUTTONHOVER;

		Invalidate();
	}
	else
	{
		if(-1 != m_msMouse.nOldIndex)
		{
			m_vctSafeItem[m_msMouse.nOldIndex].nBtnState = MS_LBUTTONNORMAL;

			if(FALSE == m_msMouse.bDown)
				m_msMouse.nOldIndex = -1;

			Invalidate();
		}
	}	

	return 0;
}

LRESULT CSafeRepairCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(-1 != m_msMouse.nOldIndex)
	{
		m_vctSafeItem[m_msMouse.nOldIndex].nBtnState = MS_LBUTTONNORMAL;
		m_msMouse.nOldIndex = -1;

		Invalidate();
	}
	return 0;
}

void CSafeRepairCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT			event1		= {0};

	event1.cbSize = sizeof(TRACKMOUSEEVENT);
	event1.dwHoverTime = 1;
	event1.hwndTrack = m_hWnd;
	event1.dwFlags = TME_HOVER | TME_LEAVE;

	TrackMouseEvent(&event1);

// 	CListBox::OnMouseMove(nFlags, point);
}

CRect CSafeRepairCtrl::CalButtonRect(CRect rect)
{
	CRect			rtRet	= rect;

	rtRet.left = rtRet.right - 5 - m_sizeBtn.cx;
	rtRet.top = rtRet.top + rtRet.Height() / 2 - m_sizeBtn.cy / 2;
	rtRet.right = rtRet.left + m_sizeBtn.cx;
	rtRet.bottom = rtRet.top + m_sizeBtn.cy;

	return rtRet;
}

BOOL CSafeRepairCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect		rtItems;
	CRect		rtClient;

	GetClientRect(&rtClient);
	if(LB_ERR == GetItemRect(GetCount() - 1, &rtItems))
		return FALSE;
	if(rtItems.bottom >= rtClient.bottom)
		return FALSE;

	rtClient.top = rtItems.bottom;
	pDC->FillSolidRect(&rtClient, RGB(0xff, 0xff, 0xff));

	return FALSE;
}
// 添加安全选项
void CSafeRepairCtrl::AddSafeItem(BOOL bSafe, LPCTSTR lpTitle, LPCTSTR lpDescrib,
								  LPCTSTR lpBtnText, int nBtnID)
{
	SAFEDATA		safe		= {0};

	safe.bSafe = bSafe;
	safe.strTitle = lpTitle;
	safe.strDescrib = lpDescrib;
	safe.nBtnID = nBtnID;
	safe.nBtnState = 0;
	safe.strBtnText = lpBtnText;

	m_vctSafeItem.push_back(safe);

	AddString("");
	SetItemHeight(0, m_nItemHeight);
}

// 清空安全选项
void CSafeRepairCtrl::ClearSafeItem()
{
	memset(&m_msMouse, 0, sizeof(m_msMouse));
	m_msMouse.nOldIndex = -1;
	m_vctSafeItem.clear();
	ResetContent();
}

// 设置图片
void CSafeRepairCtrl::SetImageList(UINT nSafeImg /* = -1 */, UINT nBtn /* = -1 */)
{
	CBitmap		bmp;
	BITMAP		bit;

	if(-1 != nSafeImg)
	{

		if(NULL != m_imgSafeState.GetSafeHandle())
			m_imgSafeState.DeleteImageList();

		bmp.LoadBitmap(nSafeImg);

		bmp.GetBitmap(&bit);
		m_imgSafeState.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgSafeState.Add(&bmp, RGB(0xff, 0x0, 0xff));
		
		bmp.DeleteObject();

		m_sizeState.cx = m_sizeState.cy = bit.bmHeight;
	}

	if(-1 != nSafeImg)
	{
		if(NULL != m_imgBtn.GetSafeHandle())
			m_imgBtn.DeleteImageList();

		bmp.LoadBitmap(nBtn);

		bmp.GetBitmap(&bit);
		m_imgBtn.Create(bit.bmWidth / 4, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgBtn.Add(&bmp, RGB(0xff, 0x0, 0xff));

		bmp.DeleteObject();

		m_sizeBtn.cy = bit.bmHeight;
		m_sizeBtn.cx = bit.bmWidth / 4;
	}
}

