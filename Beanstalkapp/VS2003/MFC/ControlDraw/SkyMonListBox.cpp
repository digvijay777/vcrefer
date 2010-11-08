// CtrlListBox.cpp : implementation file
//

#include "stdafx.h"
#include "SkyMonListBox.h"
#include ".\skymonlistbox.h"

#define MONLIST_HEIGHT_SELECT			60
#define MONLIST_HEIGHT_NORMAL			30

#define MONLIST_SELECT_BKCOLOR			RGB(0xEF, 0xEF, 0xEF)

#define CheckMask(mk, ck)		(( (mk) & (ck) ) == (ck) )
// CCtrlListBox

IMPLEMENT_DYNAMIC(CSkyMonListBox, CListBox)
CSkyMonListBox::CSkyMonListBox()
{
	m_nCurSel = -1;
	m_uMsgClick = -1;
	m_hHand = LoadCursor(NULL, IDC_HAND);
}

CSkyMonListBox::~CSkyMonListBox()
{
}


BEGIN_MESSAGE_MAP(CSkyMonListBox, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// 添加数据
int CSkyMonListBox::AddMonItem(PMONLISTDATA pData, int nMask /* = MD_ALL */)
{
	int				nItem	= 0;
	MONLISTDATA		monData;
	CRect			rect(-1, -1, -1, -1);

	CListBox::AddString("");
	nItem = CListBox::GetCount() - 1;
	
	if(nItem < 0)
		return -1;	// 添加失败

	m_vctData.push_back(monData);
	m_vctRect.push_back(rect);
	if(FALSE == SetMonItem(nItem, pData, nMask))
	{
		CListBox::DeleteString(nItem);
		return -1;
	}

	return nItem;
}

void CSkyMonListBox::DeleteAllItem()
{
	ResetContent();

	m_vctData.clear();
	m_vctRect.clear();
}

// 设置数据
BOOL CSkyMonListBox::SetMonItem(int nItem, PMONLISTDATA pData, int nMask)
{
	if(nItem < 0 || nItem >= m_vctData.size())
		return FALSE;

	MONLISTDATA&	data	= m_vctData[nItem];

	if(CheckMask(nMask, MD_TITLE))
		data.strTitle = pData->strTitle;
	if(CheckMask(nMask, MD_DETAIL))
		data.strDetail = pData->strDetail;
	if(CheckMask(nMask, MD_ON))
		data.strOn = pData->strOn;
	if(CheckMask(nMask, MD_OFF))
		data.strOff = pData->strOff;
	if(CheckMask(nMask, MD_TURNON))
		data.strBtnTurnOn = pData->strBtnTurnOn;
	if(CheckMask(nMask, MD_TURNOFF))
		data.strBtnTurnOff = pData->strBtnTurnOff;
	if(CheckMask(nMask, MD_IMAGE))
		data.nImage = pData->nImage;
	if(CheckMask(nMask, MD_RUNNING))
		data.bRunning = pData->bRunning;

	CRect		rtItem;

	GetItemRect(nItem, &rtItem);
	InvalidateRect(&rtItem, TRUE);
	return TRUE;
}
int CSkyMonListBox::SetCallBackMsg(UINT uMsg)
{
	int		nRet		= m_uMsgClick;

	m_uMsgClick = uMsg;
	return nRet;
}

BOOL CSkyMonListBox::SetBitmaps(UINT uMaxState, UINT uMinState, UINT uSelBk)
{
	CBitmap			bmpMaxState;
	CBitmap			bmpMinState;
	CBitmap			bmpSelBk;

	if(-1 != uMaxState)
		bmpMaxState.LoadBitmap(uMaxState);
	if(-1 != uMinState)
		bmpMinState.LoadBitmap(uMinState);
	if(-1 != uSelBk)
		bmpSelBk.LoadBitmap(uSelBk);

	return SetBitmaps(&bmpMaxState, &bmpMinState, &bmpSelBk);
}

BOOL CSkyMonListBox::SetBitmaps(CBitmap* pMaxState, CBitmap* pMinState, CBitmap* pSelBk)
{
	if(NULL != pMaxState && NULL != pMaxState->GetSafeHandle())
	{
		BITMAP		bmp		= {0};

		if(NULL != m_imgMaxState.GetSafeHandle())
			m_imgMaxState.DeleteImageList();
		pMaxState->GetBitmap(&bmp);
		m_imgMaxState.Create(bmp.bmHeight, bmp.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
		m_imgMaxState.Add(pMaxState, RGB(0xff, 0x0, 0xff));
	}
	if(NULL != pMinState && NULL != pMinState->GetSafeHandle())
	{
		BITMAP		bmp		= {0};

		if(NULL != m_imgMinState.GetSafeHandle())
			m_imgMinState.DeleteImageList();
		pMinState->GetBitmap(&bmp);
		m_imgMinState.Create(bmp.bmHeight, bmp.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
		m_imgMinState.Add(pMinState, RGB(0xff, 0x0, 0xff));
	}
	if(NULL != pSelBk && NULL != pSelBk->GetSafeHandle())
	{
		BITMAP		bmp		= {0};

		if(NULL != m_imgSelBk.GetSafeHandle())
			m_imgSelBk.DeleteImageList();
		pSelBk->GetBitmap(&bmp);
		m_imgSelBk.Create(bmp.bmWidth / 3, bmp.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
		m_imgSelBk.Add(pSelBk, RGB(0xff, 0x0, 0xff));
	}

	return TRUE;
}


void CSkyMonListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int				nItem		= lpDrawItemStruct->itemID;

	if(nItem < 0 || nItem >= m_vctData.size())
		return;

	CRect			rtItem		= lpDrawItemStruct->rcItem;
	CDC*			pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	MONLISTDATA&	monData		= m_vctData[nItem];
	CDC				memDC;
	CBitmap			bitmap;
	CBitmap*		pMap;

	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rtItem.Width(), rtItem.Height());
	pMap = memDC.SelectObject(&bitmap);

	if(m_nCurSel == lpDrawItemStruct->itemID)
		DrawItem_Select(pDC, rtItem, monData, &m_vctRect[nItem]);
	else
		DrawItem_Normal(pDC, rtItem, monData, &m_vctRect[nItem]);

	pDC->BitBlt(rtItem.left, rtItem.right, rtItem.Width(), rtItem.Height(),
		&memDC, rtItem.Width(), rtItem.Height(), SRCCOPY);
	memDC.SelectObject(pMap);
}

int CSkyMonListBox::DrawItem_Select(CDC* pDC, CRect& rect, MONLISTDATA& monData, CRect* pRetBtn /* = NULL */)
{
	int			nDefHeight			= MONLIST_HEIGHT_NORMAL;
	IMAGEINFO	info				= {0};
	CRect		rtDraw				= rect;

	rtDraw.DeflateRect(5, 3, 5, 3);
	pDC->SetBkMode(TRANSPARENT);

	// 背景
	if(NULL == m_imgSelBk.GetSafeHandle())
	{
		pDC->FillSolidRect(&rect, MONLIST_SELECT_BKCOLOR);
	}
	else
	{
		IMAGEINFO		bkInfo		= {0};
		int				nWidth;

		m_imgSelBk.GetImageInfo(0, &bkInfo);
		nWidth = bkInfo.rcImage.right - bkInfo.rcImage.left;
		for(int i = rect.left; i < (rect.right + nWidth - 1); i += nWidth)
		{
			m_imgSelBk.Draw(pDC, 1, CPoint(i, rect.top), ILD_TRANSPARENT);
		}
		m_imgSelBk.Draw(pDC, 0, rect.TopLeft(), ILD_TRANSPARENT);
		m_imgSelBk.Draw(pDC, 2, CPoint(rect.right - nWidth, rect.top), ILD_TRANSPARENT);
	}

	// 图标
	if(NULL != m_imgMaxState.GetSafeHandle())
	{
		m_imgMaxState.GetImageInfo(0, &info);
		m_imgMaxState.Draw(pDC, monData.nImage
			, CPoint(rtDraw.left, rtDraw.top + (rtDraw.Height() - info.rcImage.bottom + info.rcImage.top) / 2)
			, ILD_TRANSPARENT);
		rtDraw.left += info.rcImage.right - info.rcImage.left + 5;
	}
	// 按钮
	CFont		*pFont, font;
	LOGFONT		logFont			= {0};
	CRect		rtBtn;
	CString&	strBtn			= monData.bRunning?monData.strBtnTurnOff:monData.strBtnTurnOn;

	GetFont()->GetLogFont(&logFont);
	logFont.lfUnderline = TRUE;
	font.CreateFontIndirect(&logFont);
	pFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0x0, 0x0, 0xff));

	pDC->DrawText(strBtn, &rtBtn, DT_CALCRECT);
	rtBtn.MoveToX(rtDraw.right - 40);
	rtBtn.MoveToY(rtDraw.top + (rtDraw.Height() - rtBtn.Height()) / 2);
	if(NULL != pRetBtn)
		*pRetBtn = rtBtn;
	rtDraw.right -= (rtBtn.Width() + 5);
	pDC->DrawText(strBtn, &rtBtn, DT_LEFT);
	pDC->SelectObject(pFont);

	// 绘制主文本
	CRect		rtTitle;
	CFont		bordFont;

	logFont.lfUnderline = FALSE;
	logFont.lfWeight = 700;
	bordFont.CreateFontIndirect(&logFont);
	pFont = pDC->SelectObject(&bordFont);
	pDC->SetTextColor(RGB(0x0, 0x0, 0x0));
	pDC->DrawText(monData.strTitle, &rtDraw, DT_LEFT);
	pDC->DrawText(monData.strTitle, &rtTitle, DT_CALCRECT);

	// 绘制状态
	CRect		rtStatus		= rtDraw;

	rtStatus.left += rtTitle.Width();
	pDC->SetTextColor(monData.bRunning?RGB(0x0, 0xff, 0x0):RGB(0xff, 0x0, 0x0));
	pDC->DrawText(monData.bRunning?monData.strOn:monData.strOff, &rtStatus, DT_LEFT);
	pDC->SelectObject(pFont);
	// 绘制细节
	CRect		rtDetail		= rtDraw;

	rtDetail.top += rtTitle.Height() + 2;
	pDC->SetTextColor(RGB(0x0, 0x0, 0x0));
	pDC->DrawText(monData.strDetail, &rtDetail, DT_WORDBREAK);

	return MONLIST_HEIGHT_SELECT;
}

int CSkyMonListBox::DrawItem_Normal(CDC* pDC, CRect& rect, MONLISTDATA& monData, CRect* pRetBtn /* = NULL */)
{
	int			nDefHeight			= MONLIST_HEIGHT_NORMAL;
	IMAGEINFO	info				= {0};
	CRect		rtDraw				= rect;

	rtDraw.DeflateRect(5, 3, 5, 3);
	pDC->SetBkMode(TRANSPARENT);
	// 背景
	pDC->FillSolidRect(&rect, RGB(0xff, 0xff, 0xff));
	// 图标
	if(NULL != m_imgMinState.GetSafeHandle())
	{
		m_imgMinState.GetImageInfo(0, &info);
		m_imgMinState.Draw(pDC, monData.nImage
			, CPoint(rtDraw.left, rtDraw.top + (rtDraw.Height() - info.rcImage.bottom + info.rcImage.top) / 2)
			, ILD_TRANSPARENT);
		rtDraw.left += info.rcImage.right - info.rcImage.left + 5;
	}
	// 按钮
	CFont		*pFont, font;
	LOGFONT		logFont			= {0};
	CRect		rtBtn;
	CString&	strBtn			= monData.bRunning?monData.strBtnTurnOff:monData.strBtnTurnOn;

	GetFont()->GetLogFont(&logFont);
	logFont.lfUnderline = TRUE;
	font.CreateFontIndirect(&logFont);
	pFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0x0, 0x0, 0xff));
	
	pDC->DrawText(strBtn, &rtBtn, DT_CALCRECT);
	//rtBtn.MoveToX(rtDraw.right - rtBtn.Width());
	rtBtn.MoveToX(rtDraw.right - 40);
	rtBtn.MoveToY(rtDraw.top + (rtDraw.Height() - rtBtn.Height()) / 2);
	if(NULL != pRetBtn)
		*pRetBtn = rtBtn;
	rtDraw.right -= (rtBtn.Width() + 5);
	pDC->DrawText(strBtn, &rtBtn, DT_LEFT);
	pDC->SelectObject(pFont);
	
	// 绘制主文本
	CRect		rtTitle;

	pDC->SetTextColor(RGB(0x0, 0x0, 0x0));
	pDC->DrawText(monData.strTitle, &rtDraw, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->DrawText(monData.strTitle, &rtTitle, DT_CALCRECT);
	// 绘制状态
	CRect		rtStatus		= rtDraw;

	rtStatus.left += rtTitle.Width();
	pDC->SetTextColor(monData.bRunning?RGB(0x0, 0xff, 0x0):RGB(0xff, 0x0, 0x0));
	pDC->DrawText(monData.bRunning?monData.strOn:monData.strOff, &rtStatus, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	return nDefHeight;
}
void CSkyMonListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = GetMonItemHeight(lpMeasureItemStruct->itemID
		, m_nCurSel == lpMeasureItemStruct->itemID);
}
void CSkyMonListBox::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CListBox::PreSubclassWindow();
}
int CSkyMonListBox::GetMonItemHeight(int nItem, BOOL bSelect)
{
	if(bSelect)
	{
		if(NULL != m_imgSelBk.GetSafeHandle())
		{
			IMAGEINFO		info		= {0};

			m_imgSelBk.GetImageInfo(0, &info);
			return info.rcImage.bottom - info.rcImage.top;
		}

		return MONLIST_HEIGHT_SELECT;
	}

	return MONLIST_HEIGHT_NORMAL;
}

void CSkyMonListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	int			nLink		= GetMouseOnLink(point);

	if(nLink >= 0)
	{
		SetCursor(m_hHand);
		if(-1 != m_uMsgClick)
			GetParent()->SendMessage(m_uMsgClick, nLink, m_vctData[nLink].bRunning);
		return;
	}

	CListBox::OnLButtonDown(nFlags, point);

	if(HitTest(point) != m_nCurSel)
	{
		m_nCurSel = HitTest(point);

		SetRedraw(FALSE);
		ResetContent();
		for(int i = 0; i < m_vctData.size(); i++)
			AddString("");
		SetRedraw(TRUE);
	}
}

int CSkyMonListBox::HitTest(CPoint point)
{
	for(int i = GetTopIndex(); i < m_vctData.size(); i++)
	{
		CRect		rect;

		GetItemRect(i, &rect);
		if(rect.PtInRect(point))
			return i;
	}
	return -1;
}
// 鼠标是否在连接上
int CSkyMonListBox::GetMouseOnLink(CPoint& point)
{
	for(int i = GetTopIndex(); i < m_vctRect.size(); i++)
	{
		if(m_vctRect[i].PtInRect(point))
			return i;
	}

	return -1;
}


void CSkyMonListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if(GetMouseOnLink(point) >= 0)
	{
		SetCursor(m_hHand);
	}

	CListBox::OnMouseMove(nFlags, point);
}

void CSkyMonListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(GetMouseOnLink(point) >= 0)
	{
		SetCursor(m_hHand);
	}

	CListBox::OnLButtonUp(nFlags, point);
}
