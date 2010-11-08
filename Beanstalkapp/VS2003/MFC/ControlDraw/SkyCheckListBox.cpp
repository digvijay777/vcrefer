// SkyCheckListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "SkyCheckListBox.h"
#include ".\skychecklistbox.h"
#include "MemDC.h"

// CSkyCheckListBox

IMPLEMENT_DYNAMIC(CSkyCheckListBox, CListBox)
CSkyCheckListBox::CSkyCheckListBox()
{
	m_nItemHeight = 20;
	m_colText = RGB(0x0, 0x0, 0x0);
}

CSkyCheckListBox::~CSkyCheckListBox()
{
}


BEGIN_MESSAGE_MAP(CSkyCheckListBox, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CSkyCheckListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
	ModifyListStyle();
}

BOOL CSkyCheckListBox::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL		bRet		= CListBox::PreCreateWindow(cs);

	ModifyListStyle();

	return bRet;
}

void CSkyCheckListBox::ModifyListStyle()
{

}

// 设置状态图标
void CSkyCheckListBox::SetImageList(UINT nIDState /* = -1 */)
{
	CBitmap			bmp;
	BITMAP			bit		= {0};

	if(-1 != nIDState)
	{
		if(NULL != m_imgState.GetSafeHandle())
			m_imgState.DeleteImageList();

		bmp.LoadBitmap(nIDState);
		bmp.GetBitmap(&bit);

		m_imgState.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgState.Add(&bmp, RGB(0xff, 0x0, 0xff));

		bmp.DeleteObject();
	}
}

// 选择全部
void CSkyCheckListBox::CheckAll(BOOL bChecked /* = TRUE */)
{
	for(int i = 0; i < m_vctItem.size(); i++)
	{
		m_vctItem[i].bChecked = bChecked;
	}

	Invalidate();
}

// 得到插件状态
BOOL CSkyCheckListBox::GetItemChecked(int nIndex)
{
	if(nIndex < 0 || nIndex > (m_vctItem.size() - 1))
		return FALSE;

	return m_vctItem[nIndex].bChecked;
}
void CSkyCheckListBox::SetTextColor(COLORREF colText /* = RGB */)
{
	m_colText = colText;
}
void CSkyCheckListBox::AddItem(LPCSTR lpText)
{
	ITEMDATA		data		= {0};

	data.bChecked = FALSE;
	data.strText = lpText;

	m_vctItem.push_back(data);
	int		nIndex	= AddString("");

	SetItemHeight(nIndex, m_nItemHeight);
}

void CSkyCheckListBox::ClearItem()
{
	m_vctItem.clear();
	ResetContent();
}

void CSkyCheckListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_vctItem.size() < 1)
		return;
	DrawItem_ShowList(lpDrawItemStruct);
}

// 绘制列表
void CSkyCheckListBox::DrawItem_ShowList(LPDRAWITEMSTRUCT pItem)
{
	CRect			rtItem(pItem->rcItem);
	ITEMDATA&		data							= m_vctItem[pItem->itemID];
	CDC*			pDC								= CDC::FromHandle(pItem->hDC);
	CMemDC			memDC(pDC, rtItem);
	CRect			rtMem;
	CRect			rtText;

	rtMem = rtItem;
	rtMem.MoveToXY(0, 0);
	rtText = rtMem;
	// 背景
	memDC.FillSolidRect(&rtMem, RGB(0xff, 0xff, 0xff));
	// 状态图标
	if(NULL != m_imgState.GetSafeHandle())
	{
		IMAGEINFO		info;
		CRect			rtCheck;

		m_imgState.GetImageInfo(0, &info);
		rtCheck = info.rcImage;
		rtCheck.MoveToX(10);
		rtCheck.MoveToY(rtMem.top + (rtMem.Height() - rtCheck.Height()) / 2);
		m_imgState.Draw(&memDC, data.bChecked, rtCheck.TopLeft(), ILD_TRANSPARENT);
		rtText.left += rtCheck.Width() + 15;
	}
	// 标题
	TEXTMETRIC				txtMetric		= {0};
	IMAGEINFO				info			= {0};
	CFont*					pOldDCFont		= NULL;

	pOldDCFont = memDC.SelectObject(GetFont());
	memDC.SetTextColor(m_colText);
	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(data.strText, &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	memDC.SelectObject(pOldDCFont);
}

// 得到状态图标位置
CRect CSkyCheckListBox::CalCheckBoxRect(CRect& itRect)
{
	CRect		rect;
	IMAGEINFO	info			= {0};

	m_imgState.GetImageInfo(0, &info);
	rect = info.rcImage;
	rect.MoveToX(itRect.left + 10);
	rect.MoveToY(itRect.top + (itRect.Height() - rect.Height()) / 2);
	return rect;
}

// 左键接下
void CSkyCheckListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL	bBOOL;
	int nIndex = ItemFromPoint(point,bBOOL);
	if(nIndex == 0xFFFF)
		return ;

	// 	m_bMouseDown = TRUE;
	CRect	rect;
	GetItemRect(nIndex,rect);

	CRect rectCheckIcon = CalCheckBoxRect(rect);

	if(rectCheckIcon.PtInRect(point))
	{
		ITEMDATA&		plug		= m_vctItem[nIndex];

		plug.bChecked = !plug.bChecked;
		Invalidate();
	}

	SetFocus();
	// CListBox::OnLButtonDown(nFlags, point);
}

BOOL CSkyCheckListBox::OnEraseBkgnd(CDC* pDC)
{
	CRect		rtItems;
	CRect		rtClient;

	GetClientRect(&rtClient);
	if(LB_ERR == GetItemRect(GetCount() - 1, &rtItems))
		return FALSE;
	if(rtItems.bottom >= rtClient.bottom)
		return FALSE;

	rtClient.top = rtItems.bottom;
	pDC->FillSolidRect(&rtClient, RGB(0xff, 0xff, 0xff));
	return TRUE;

	return CListBox::OnEraseBkgnd(pDC);
}

void CSkyCheckListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	OnLButtonDown(nFlags, point);
	// CListBox::OnLButtonDblClk(nFlags, point);
}
