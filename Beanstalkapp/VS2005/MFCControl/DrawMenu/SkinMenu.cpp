// SkinMenu.cpp : implementation file
//

#include "stdafx.h"
#include "DrawMenu.h"
#include "SkinMenu.h"

#define WNDPROP_SKINMENU		_T("__skin_menu__")


#ifndef arrayof
#define arrayof(x)		( (sizeof((x)) / sizeof((x)[0])) )
#endif

//////////////////////////////////////////////////////////////////////////
// CSkinMenu
IMPLEMENT_DYNAMIC(CSkinMenu, CMenu)

CSkinMenu::CSkinMenu()
{
	memset(m_colFrames, 0, sizeof(m_colFrames));
}

CSkinMenu::~CSkinMenu()
{
}

// CSkinMenu message handlers
void CSkinMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item
	CDC*			pDC					= CDC::FromHandle(lpDrawItemStruct->hDC);
	int				nSaveDC				= pDC->SaveDC();
	CRect			rect(&lpDrawItemStruct->rcItem);
	MENUITEMINFO	info				= { 0 };
	BOOL			bSplit				= FALSE;
	LPCTSTR			pTest				= (LPCTSTR)lpDrawItemStruct->itemData;
	COLORREF		col					= RGB(0x0, 0x0, 0x0);

	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_TYPE;
	GetMenuItemInfo(lpDrawItemStruct->itemID, &info);
	if(info.fType & MFT_SEPARATOR)
		bSplit = TRUE;
	// 绘制外边框
	DrawFrame(lpDrawItemStruct->hDC);
	// 绘制分隔条
	if(NULL != m_imgList.GetSafeHandle())
	{
		if(bSplit)
		{
			m_imgList.Draw(pDC, 2, rect.TopLeft(), ILD_TRANSPARENT);
		}
		else if(lpDrawItemStruct->itemState & ODS_GRAYED)
		{
			m_imgList.Draw(pDC, 3, rect.TopLeft(), ILD_TRANSPARENT);
			col = RGB(0xcc, 0xcc, 0xcc);
		}
		else if(lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			m_imgList.Draw(pDC, 1, rect.TopLeft(), ILD_TRANSPARENT);
			col = RGB(0xff, 0xff, 0xff);
		}
		else
		{
			m_imgList.Draw(pDC, 0, rect.TopLeft(), ILD_TRANSPARENT);
		}
	}
	// 画文字
	if(FALSE == bSplit)
	{
		pDC->SetTextColor(col);
		pDC->SetBkMode(TRANSPARENT);
		rect.left += 20;
		pDC->DrawText(pTest, _tcslen(pTest), &rect, DT_SINGLELINE | DT_VCENTER);
	}

	// 绘制背景
	pDC->RestoreDC(nSaveDC);
}
// 绘制边框
void CSkinMenu::DrawFrame(HDC hDC)
{
	CWnd*		pWnd				= CWnd::FromHandle(WindowFromDC(hDC));
	CDC*		pDC					= NULL;
	CRect		rtWnd(0, 0, 0, 0);
	CRect		rtClient(0, 0, 0, 0);
	int			nSaveDC				= 0;
	CRgn		rgn1;
	CRgn		rgn2;

	if(NULL == pWnd)
		return;
	pDC = pWnd->GetWindowDC();
	if(NULL == pDC)
		return;
	nSaveDC = pDC->SaveDC();
	pWnd->GetWindowRect(&rtWnd);
	pWnd->GetClientRect(&rtClient);
	pWnd->ClientToScreen(&rtClient);
	rtClient.MoveToXY(rtClient.left - rtWnd.left, rtClient.top - rtWnd.top);
	rtWnd.MoveToXY(0, 0);
	rgn1.CreateRectRgnIndirect(&rtWnd);
	rgn2.CreateRectRgnIndirect(&rtClient);
	rgn1.CombineRgn(&rgn1, &rgn2, RGN_DIFF);
	pDC->SelectObject(&rgn1);
	// 绘制
	pDC->FillSolidRect(&rtWnd, m_colFrames[0]);
	rtWnd.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(&rtWnd, m_colFrames[1]);
	rtWnd.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(&rtWnd, m_colFrames[2]);
	// 还原
	pDC->RestoreDC(nSaveDC);
	pWnd->ReleaseDC(pDC);
}
// 确定高度
void CSkinMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	MENUITEMINFO		info		= {0};

	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_TYPE;
	GetMenuItemInfo(lpMeasureItemStruct->itemID, &info);
	lpMeasureItemStruct->itemWidth = m_sizeItem.cx - 12; // 减12px未知原因，以等证实
	lpMeasureItemStruct->itemHeight = (MFT_SEPARATOR & info.fType)?8:m_sizeItem.cy;
}
// 加载图标
BOOL CSkinMenu::LoadImages(int nItem)
{
	CBitmap		bmpItem;
	CBitmap		bmpBk;

	if(-1 != nItem)
		bmpItem.LoadBitmap(nItem);

	return SetImages((HBITMAP)bmpItem.Detach());
}
// 加载位图
BOOL CSkinMenu::SetImages(HBITMAP hItem /* = NULL */)
{
	// 加载项背景
	if(NULL != hItem)
	{
		if(NULL != m_imgList.GetSafeHandle())
			m_imgList.DeleteImageList();

		BITMAP		bInfo			= {0};
		CBitmap		bmp;
		IMAGEINFO	iInfo			= {0};
		
		bmp.Attach(hItem);
		::GetObject(hItem, sizeof(BITMAP), &bInfo);
		m_sizeItem.SetSize(bInfo.bmWidth / 5, bInfo.bmHeight);
		m_imgList.Create(m_sizeItem.cx, m_sizeItem.cy, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgList.Add(&bmp, RGB(0xff, 0x0, 0xff));
		m_imgList.GetImageInfo(4, &iInfo);
		SetFrameColor(hItem);
	}

	return FALSE;
}

/*
 *	设置边框颜色
 */
void CSkinMenu::SetFrameColor(HBITMAP hImage)
{
	if(NULL == hImage)
		return;

	CDC			memDC;
	HBITMAP		oldBmp;
	BITMAP		bmp			= {0};

	GetObject(hImage, sizeof(BITMAP), &bmp);
	memDC.CreateCompatibleDC(NULL);
	oldBmp = (HBITMAP)memDC.SelectObject(hImage);
	m_colFrames[0] = memDC.GetPixel(bmp.bmWidth - 1, 0);
	m_colFrames[1] = memDC.GetPixel(bmp.bmWidth - 1, 1);
	m_colFrames[2] = memDC.GetPixel(bmp.bmWidth - 1, 2);
	memDC.SelectObject(oldBmp);
}