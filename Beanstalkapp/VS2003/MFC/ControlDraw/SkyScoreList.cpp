// SkyScoreList.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "SkyScoreList.h"
#include ".\skyscorelist.h"


// CSkyScoreList

IMPLEMENT_DYNAMIC(CSkyScoreList, CListBox)
CSkyScoreList::CSkyScoreList()
{
	m_colGroup = RGB(0x0, 0x0, 0x0);
	m_colItemText = RGB(0x0, 0x0, 0x0);
	m_nCheckingState = 0;

	memset(&m_infoList, 0, sizeof(m_infoList));
	memset(&m_infoCheckingState, 0, sizeof(m_infoCheckingState));
}

CSkyScoreList::~CSkyScoreList()
{
}


BEGIN_MESSAGE_MAP(CSkyScoreList, CListBox)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CSkyScoreList::PreSubclassWindow()
{
// 	ASSERT((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_SORT)) == 0);

	LOGFONT		logFont;

	GetFont()->GetLogFont(&logFont);
	logFont.lfWeight = 700;
	m_fontBlod.CreateFontIndirect(&logFont);

	CListBox::PreSubclassWindow();
}
void CSkyScoreList::SetImageList(UINT uImg, UINT nCheckingState /* = -1 */)
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
int CSkyScoreList::AddItem(LPCSTR lpText, BOOL Group /* = FALSE */, int nState /* = 0 */)
{
	SCOREDATA		data;

	data.bGroup = Group;
	data.nState = nState;
	data.strText = lpText;

	m_vctData.push_back(data);
	AddString("");
	SetItemHeight(m_vctData.size()-1, 20);
	return m_vctData.size() - 1;
}
void CSkyScoreList::ClearItem()
{
	m_vctData.clear();
	ResetContent();
}
void CSkyScoreList::SetTextColor(COLORREF colText /* = RGB */, COLORREF colGroup)
{
	m_colItemText = colText;
	m_colGroup = colGroup;
}
void CSkyScoreList::SetItemState(int nIndex, int nState)
{
	if(nIndex < 0 || nIndex >= m_vctData.size())
		return;

	m_vctData[nIndex].nState = nState;
	CRect		rect;
	GetItemRect(nIndex, &rect);
	InvalidateRect(&rect);
}
void CSkyScoreList::SetItemText(int nIndex, LPCSTR lpText)
{
	if(nIndex < 0 || nIndex >= m_vctData.size())
		return;

	m_vctData[nIndex].strText = lpText;

	CRect		rect;
	GetItemRect(nIndex, &rect);
	InvalidateRect(&rect);
}
void CSkyScoreList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
		DrawItem_Item(pDC, data.strText.GetBuffer(), rect, data.nState);
	}
}

void CSkyScoreList::DrawItem_Group(CDC *pDC, LPSTR lpText, CRect rect)
{
	pDC->FillSolidRect(&rect, RGB(0xe2, 0xed, 0xee));
	rect.DeflateRect(2, 0, 0, 0);
	
	CFont*		pFont		= pDC->SelectObject(&m_fontBlod);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colGroup);
	pDC->DrawText(lpText, strlen(lpText), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pFont);
}

void CSkyScoreList::DrawItem_Item(CDC* pDC, LPSTR lpText, CRect rect, int nState)
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
}

void CSkyScoreList::OnTimer(UINT nIDEvent)
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
