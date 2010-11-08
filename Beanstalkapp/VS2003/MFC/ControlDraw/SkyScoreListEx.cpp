// SkyScoreList.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "SkyScoreListEx.h"
#include ".\skyscorelistex.h"

// CSkyScoreListEx

IMPLEMENT_DYNAMIC(CSkyScoreListEx, CListBox)
CSkyScoreListEx::CSkyScoreListEx()
{
	m_colGroup = RGB(0x0, 0x0, 0x0);
	m_colItemText = RGB(0x0, 0x0, 0x0);
	m_nCheckingState = 0;
	m_uLinkMsg = 0;
	m_hwndLinkMsg = NULL;
	m_handCursor = LoadCursor(NULL, IDC_HAND);

	memset(&m_infoList, 0, sizeof(m_infoList));
	memset(&m_infoCheckingState, 0, sizeof(m_infoCheckingState));
}

CSkyScoreListEx::~CSkyScoreListEx()
{
}


BEGIN_MESSAGE_MAP(CSkyScoreListEx, CListBox)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CSkyScoreListEx::PreSubclassWindow()
{
// 	ASSERT((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_SORT)) == 0);

	LOGFONT		logFont;

	GetFont()->GetLogFont(&logFont);
	logFont.lfWeight = 700;
	m_fontBlod.CreateFontIndirect(&logFont);

	CListBox::PreSubclassWindow();
}
void CSkyScoreListEx::SetImageList(UINT uImg, UINT nCheckingState /* = -1 */)
{
	if(NULL != m_imgList.GetSafeHandle())
		m_imgList.DeleteImageList();

	CBitmap		bmp;
	BITMAP		bit;

	bmp.LoadBitmap(uImg);
	bmp.GetBitmap(&bit);
	m_imgList.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_imgList.Add(&bmp, RGB(0xff, 0x0, 0xff));
	bmp.DeleteObject();
	m_imgList.GetImageInfo(0, &m_infoList);

	if(-1 != nCheckingState)
	{
		if(NULL != m_imgCheckingState.GetSafeHandle())
			m_imgCheckingState.DeleteImageList();

		bmp.LoadBitmap(nCheckingState);
		bmp.GetBitmap(&bit);
		m_imgCheckingState.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgCheckingState.Add(&bmp, RGB(0xff, 0x0, 0xff));
		bmp.DeleteObject();
		m_imgCheckingState.GetImageInfo(0, &m_infoCheckingState);

		SetTimer(1, 200, NULL);
	}
}
int CSkyScoreListEx::AddItem(LPCSTR lpText, BOOL Group /* = FALSE */, int nState /* = 0 */, LPCSTR lpLink /* = NULL */)
{
	SCOREDATA		data;

	data.bGroup = Group;
	data.nState = nState;
	data.strText = lpText;
	data.rtLink = CRect(0, 0, 0, 0);
	if(NULL != lpLink)
		data.strLink = lpLink;

	m_vctData.push_back(data);
	AddString("");
	SetItemHeight(m_vctData.size()-1, 20);
	return m_vctData.size() - 1;
}
void CSkyScoreListEx::ClearItem()
{
	m_vctData.clear();
	ResetContent();
}
BOOL CSkyScoreListEx::PtInLink(CPoint pt)
{
	int			nItem;
	BOOL		bOutSite;

	nItem = ItemFromPoint(pt, bOutSite);
	if(FALSE != bOutSite)
		return FALSE;
	if(nItem < 0 || nItem > m_vctData.size())
		return FALSE;

	CRect		rtItem;
	CRect		rtLink		= m_vctData[nItem].rtLink;

	GetItemRect(nItem, &rtItem);
	rtLink.MoveToY(rtItem.top);

	if(rtLink.PtInRect(pt))
		return TRUE;

	return FALSE;
}
void CSkyScoreListEx::SetTextColor(COLORREF colText /* = RGB */, COLORREF colGroup)
{
	m_colItemText = colText;
	m_colGroup = colGroup;
}

void CSkyScoreListEx::SetLinkMessage(UINT uLinkMsg, HWND hWnd /* = NULL */)
{
	m_uLinkMsg = uLinkMsg;
	if(NULL == hWnd)
		m_hwndLinkMsg = GetParent()->GetSafeHwnd();
	else
		m_hwndLinkMsg = hWnd;
}

void CSkyScoreListEx::SetItemState(int nIndex, int nState)
{
	if(nIndex < 0 || nIndex >= m_vctData.size())
		return;

	m_vctData[nIndex].nState = nState;
	CRect		rect;
	GetItemRect(nIndex, &rect);
	InvalidateRect(&rect);
}
void CSkyScoreListEx::SetItemText(int nIndex, LPCSTR lpText, LPCSTR lpLink /* = NULL */)
{
	if(nIndex < 0 || nIndex >= m_vctData.size())
		return;

	m_vctData[nIndex].strText = lpText;
	if(NULL != lpLink)
		m_vctData[nIndex].strLink = lpLink;
	else
		m_vctData[nIndex].strLink = "";

	CRect		rect;
	GetItemRect(nIndex, &rect);
	InvalidateRect(&rect);
}

void CSkyScoreListEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect		rect		= lpDrawItemStruct->rcItem;

	if(lpDrawItemStruct->itemID < 0 || lpDrawItemStruct->itemID >= m_vctData.size())
		return;

	SCOREDATA&	data		= m_vctData[lpDrawItemStruct->itemID];
	
	if(FALSE != data.bGroup)
	{
		DrawItem_Group(pDC, data.strText.GetBuffer(), rect);
	}
	else
	{
		DrawItem_Item(pDC, data.strText.GetBuffer(), rect, data.nState
			, ("" != data.strLink)?data.strLink.GetBuffer():NULL
			, &data.rtLink);
	}
}

void CSkyScoreListEx::DrawItem_Group(CDC *pDC, LPSTR lpText, CRect rect)
{
	pDC->FillSolidRect(&rect, RGB(0xe2, 0xed, 0xee));
	rect.DeflateRect(2, 0, 0, 0);
	
	CFont*		pFont		= pDC->SelectObject(&m_fontBlod);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colGroup);
	pDC->DrawText(lpText, strlen(lpText), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pFont);
}

void CSkyScoreListEx::DrawItem_Item(CDC* pDC, LPSTR lpText, CRect rect, int nState, LPSTR lpLink /* = NULL */
									, CRect* pRtLink /* = NULL */)
{
	rect.DeflateRect(10, 0, 0, 0);
	CRect		rtText		= rect;
	CRect		rtIcon		= rect;
	
	if(0 == nState && NULL != m_imgCheckingState.GetSafeHandle())
	{
		CRect			rtInfo;
		rtInfo = m_infoCheckingState.rcImage;
		rtIcon.top += (rect.Height() - rtInfo.Height()) / 2;
		m_imgCheckingState.Draw(pDC, m_nCheckingState, rtIcon.TopLeft(), ILD_TRANSPARENT);
		rtText.left += rtInfo.Width() + 5;
	}
	else if(NULL != m_imgList.GetSafeHandle())
	{
		CRect			rtInfo;

		rtInfo = m_infoList.rcImage;
		rtIcon.top += (rect.Height() - rtInfo.Height()) / 2;
		m_imgList.Draw(pDC, nState, rtIcon.TopLeft(), ILD_TRANSPARENT);
		rtText.left += rtInfo.Width() + 5;
	}

	CFont*		pFont		= pDC->SelectObject(GetFont());

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colGroup);
	pDC->DrawText(lpText, strlen(lpText), &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pFont);
	// 绘制连接
	if(NULL == lpLink)
	{
		*pRtLink = CRect(0, 0, 0, 0);
		return;
	}

	CRect			rtLink;
	static CFont	linkFont;

	if(NULL == linkFont.GetSafeHandle())
	{
		LOGFONT			logFont		= {0};

		GetFont()->GetLogFont(&logFont);
		logFont.lfUnderline = TRUE;
		linkFont.CreateFontIndirect(&logFont);
	}

	pFont = pDC->SelectObject(&linkFont);
	pDC->SetTextColor(RGB(0x0, 0x0, 0xff));
	pDC->DrawText(lpText, &rtLink, DT_CALCRECT);
	rtText.left += rtLink.Width() + 5;
	pDC->DrawText(lpLink, &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->DrawText(lpLink, &rtLink, DT_CALCRECT);
	pDC->SelectObject(pFont);
	rtLink.MoveToX(rtText.left);
	*pRtLink = rtLink;
}

void CSkyScoreListEx::OnTimer(UINT nIDEvent)
{
	m_nCheckingState++;
   	m_nCheckingState = m_nCheckingState % m_imgCheckingState.GetImageCount();
	for(int i = 0; i < m_vctData.size(); i++)
	{
		if(0 != m_vctData[i].nState)
			continue;

		CRect		rect;

		GetItemRect(i, &rect);
		InvalidateRect(&rect, FALSE);
	}

	CListBox::OnTimer(nIDEvent);
}

void CSkyScoreListEx::OnMouseMove(UINT nFlags, CPoint point)
{
	static HCURSOR		hCursor		= GetCursor();

	CListBox::OnMouseMove(nFlags, point);
	// 是否在连接上
	if(PtInLink(point))
	{
		TRACE("pt in link\n");
		hCursor = GetCursor();
		SetCursor(m_handCursor);
	}
	else
	{
		SetCursor(hCursor);
	}
}

void CSkyScoreListEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(PtInLink(point))
	{
		BOOL		bOutSite;

		SetCursor(m_handCursor);
		if(0 != m_uLinkMsg)
			::SendMessage(m_hwndLinkMsg, m_uLinkMsg, ItemFromPoint(point, bOutSite), 0);
		return;
	}

	CListBox::OnLButtonDown(nFlags, point);
}

void CSkyScoreListEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(PtInLink(point))
	{
		SetCursor(m_handCursor);
	}

	CListBox::OnLButtonUp(nFlags, point);
}
