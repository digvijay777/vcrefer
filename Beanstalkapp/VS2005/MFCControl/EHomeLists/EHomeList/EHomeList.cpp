// EHomeList\EHomeList.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeList.h"
#include <atlimage.h>
#include "MemDc.h"

// CEHomeList

IMPLEMENT_DYNAMIC(CEHomeList, CListCtrl)

CEHomeList::CEHomeList()
{
	m_nItemHeight = 18;
	m_pImageList[0] = NULL;
	m_pImageList[1] = NULL;
	m_pImageList[2] = NULL;
	m_bTrackMouseEvent = FALSE;
	m_dwMouseItem = -1;
	m_dwMuiltIndex = -1;
	m_hCursor[0] = ::LoadCursor(NULL, IDC_ARROW);
	m_hCursor[1] = ::LoadCursor(NULL, IDC_HAND);
}

CEHomeList::~CEHomeList()
{
}


BEGIN_MESSAGE_MAP(CEHomeList, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CEHomeList message handlers
void CEHomeList::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	DWORD		dwStyle		= 0;

	// 更改普通样式
	dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	ASSERT(dwStyle & LVS_OWNERDATA);
	dwStyle |= LVS_REPORT;				// 列表控件
	dwStyle |= LVS_OWNERDRAWFIXED;		// 自已绘制 DrawItem() 函数会被调用
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);
	// 更改扩展样式
	dwStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	dwStyle |= LVS_EX_GRIDLINES;		// 网格线
	// dwStyle |= LVS_EX_LABELTIP;			// 提示语
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwStyle);
	
	// 扩展标题栏
	if(NULL != GetHeaderCtrl())
		m_ctrlHeader.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
}

void CEHomeList::SetColumnFormat(int nColumn, EHOMELISTFORMAT fmt, HIMAGELIST hImgList)
{
	if(!IsWindow(m_ctrlHeader.GetSafeHwnd()))
		return;

	EHOMELISTCOLUMN			column		= {EHLF_Normal, NULL};

	if((int)m_vctColumnFmt.size() < m_ctrlHeader.GetItemCount())
		m_vctColumnFmt.resize(m_ctrlHeader.GetItemCount(), column);
	if(nColumn >= (int)m_vctColumnFmt.size() || nColumn < 0)
		return;

	m_vctColumnFmt[nColumn].fmt = fmt;
	m_vctColumnFmt[nColumn].hImgList = hImgList;
}
void CEHomeList::SetColumnFormat(int nColumn, EHOMELISTFORMAT fmt)
{
	SetColumnFormat(nColumn, fmt, NULL);
}
EHOMELISTFORMAT CEHomeList::GetColumnFormat(int nColumn)
{
	if(nColumn >= (int)m_vctColumnFmt.size() || nColumn < 0)
		return EHLF_Normal;
	
	return m_vctColumnFmt.at(nColumn).fmt;
}

//设置子项数据
BOOL CEHomeList::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText )
{
	LVITEM			item		= {0};

	item.mask = LVIF_TEXT;
	item.iItem = nItem;
	item.iSubItem = nSubItem;
	item.pszText = (LPTSTR)lpszText;

	return SendMessage(LVM_SETITEM, 0, (LPARAM)&item);
}

// 设置行高, 行高要在Attach之前设置，否则无效
void CEHomeList::SetItemHeight(int nHeight)
{
	m_nItemHeight = nHeight;
}
// 设置列的图标列表
void CEHomeList::SetSwitchImageList(int nSubItem, HIMAGELIST hImageList)
{
	SetColumnFormat(nSubItem, EHLF_SwitchButton, hImageList);
}
// 设置ICON图标列表
void CEHomeList::SetIconImageList(int nSubItem, HIMAGELIST hImageList)
{
	SetColumnFormat(nSubItem, EHLF_IconList, hImageList);
}
// 设置多按钮图标列表
void CEHomeList::SetMuiltButtonImageList(int nSubItem, HIMAGELIST hImageList)
{
	SetColumnFormat(nSubItem, EHLF_MuiltButton, hImageList);
}
// 消息回调
LRESULT CEHomeList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(LVM_INSERTITEM == message)
	{
		LPLVITEM			pItem		= (LPLVITEM)lParam;
		EHOMELISTITEM		item;

		item.vctstr.resize(m_ctrlHeader.GetItemCount(), _T(""));
		item.vctstr[0] = pItem->pszText;
		item.nImage = pItem->iImage;

		m_vctData.push_back(item);
		SetItemCount((int)m_vctData.size());
		return (LRESULT)m_vctData.size() - 1;
	}
	else if(LVM_SETITEM == message)
	{
		LPLVITEM			pItem		= (LPLVITEM)lParam;

		if((int)m_vctData.size() <= pItem->iItem || pItem->iItem < 0)
			return FALSE;
		if(pItem->mask & LVIF_TEXT)
		{
			if(m_vctData[pItem->iItem].vctstr.size() <= pItem->iSubItem || pItem->iSubItem < 0)
				return FALSE;
			m_vctData[pItem->iItem].vctstr[pItem->iSubItem] = pItem->pszText;
		}
		if(pItem->mask & LVIF_IMAGE)
		{
			m_vctData[pItem->iItem].nImage = pItem->iImage;
		}
		if(pItem->mask & LVIF_PARAM)
		{
			m_vctData[pItem->iItem].lpData = pItem->lParam;
		}
		return TRUE;
	}
	else if(LVM_SETIMAGELIST == message)
	{
		if(wParam < 0 || wParam > 2)
			return NULL;
		m_pImageList[wParam] = CImageList::FromHandle((HIMAGELIST)lParam); 
		return (LRESULT)m_pImageList[wParam];
	}
	else if(WM_MOUSELEAVE == message)
	{
		m_bTrackMouseEvent = FALSE;
		m_dwMouseItem = -1; 
		Invalidate(FALSE);
	}
	else if(WM_LBUTTONDOWN == message)
	{
		if(-1 != m_dwMuiltIndex)
		{
			DWORD		dwIndex		= m_dwMuiltIndex;

			m_dwMuiltIndex = -1;
			GetParent()->SendMessage( UM_EHOMELISTCLICKBUTTON, (WPARAM)GetSafeHwnd()
				, (LPARAM)MAKELONG(LOWORD(m_dwMouseItem), dwIndex) );
			return 0;
		}
		else if(-1 != m_dwMouseItem)
		{
			GetParent()->SendMessage(UM_EHOMELISTCLICKITEM, (WPARAM)GetSafeHwnd(), (LPARAM)m_dwMouseItem);
			return 0;
		}
	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}
BOOL CEHomeList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR			pNMHDR			= (LPNMHDR)lParam;


	if(TTN_NEEDTEXT == pNMHDR->code)
	{
		LPNMTTDISPINFO		pTipInfo		= (LPNMTTDISPINFO)lParam;
		LVHITTESTINFO		info		= {0};

		GetCursorPos(&info.pt);
		ScreenToClient(&info.pt);
		SubItemHitTest(&info);

		if(-1 == info.iItem || -1 == info.iSubItem)
			return FALSE;
		switch(m_vctColumnFmt[info.iSubItem].fmt)
		{
		case EHLF_Title:
			pTipInfo->lpszText = (LPTSTR)_tcsstr(m_vctData[info.iItem].vctstr[info.iSubItem].c_str(), _T("\r\n"));
			break;
		case EHLF_Normal:
			pTipInfo->lpszText = (LPTSTR)m_vctData[info.iItem].vctstr[info.iSubItem].c_str();
			break;
		default:
			pTipInfo->lpszText = NULL;
		}
		*pResult = 0;
		return TRUE;
	}

	return FALSE;
}
// 绘制普通项
void		CEHomeList::Draw_NormalItem(CDC* pDC, RECT rect, LPCTSTR lpText, UINT nColumnFmt)
{
	int			nFormat		= 0;

	nFormat |= DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS;
	if(LVCFMT_CENTER & nColumnFmt)
		nFormat |= DT_CENTER;
	if(LVCFMT_RIGHT & nColumnFmt)
		nFormat |= DT_RIGHT;

	pDC->DrawText(lpText, _tcslen(lpText), &rect, nFormat);
}

void		CEHomeList::Draw_TitleItem(CDC* pDC, RECT rect, LPCTSTR lpText)
{
	LPCTSTR			pInfo		= _tcsstr(lpText, _T("\r\n"));
	CRect			rtTitle		= rect;

	if(NULL == pInfo)
		pInfo = lpText + _tcslen(lpText);
	else
		pInfo += 2;
	if(NULL == m_FontBold.GetSafeHandle())
	{
		LOGFONT			logFont		= {0};

		GetFont()->GetLogFont(&logFont);
		logFont.lfWeight = FW_BOLD;
		m_FontBold.CreateFontIndirect(&logFont);
	}
	// 绘制标题 
	rtTitle.bottom -= rtTitle.Height() / 2;
	CFont*		pOldFont		= pDC->SelectObject(&m_FontBold);
	pDC->DrawText(lpText, pInfo - lpText, &rtTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_WORD_ELLIPSIS);
	pDC->SelectObject(pOldFont);
	// 绘制信息
	rtTitle = rect;
	rtTitle.top += rtTitle.Height() / 2;
	pDC->DrawText(pInfo, _tcslen(pInfo), &rtTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_WORD_ELLIPSIS);
}

void		CEHomeList::Draw_SwitchButtonItem(CDC* pDC, RECT& rect, int nIndex, HIMAGELIST hImageList, BOOL bMouseOver)
{
	if(NULL == hImageList)
	{
		ASSERT(NULL == hImageList);
		return;
	}

	IMAGEINFO			info		= {0};
	POINT				pt;
	CRgn				rgn;
	int					nSaveDC		= pDC->SaveDC();

	rgn.CreateRectRgnIndirect(&rect);
	pDC->SelectObject(&rgn);
	ImageList_GetImageInfo(hImageList, 0, &info);
	pt.x = rect.left + ((rect.right - rect.left) - (info.rcImage.right - info.rcImage.left)) / 2;
	pt.y = rect.top + ((rect.bottom - rect.top) - (info.rcImage.bottom - info.rcImage.top)) / 2;
	ImageList_Draw(hImageList, (bMouseOver)?nIndex * 2+1:nIndex*2, pDC->GetSafeHdc(), pt.x, pt.y, ILD_TRANSPARENT);
	rect.left = pt.x;
	rect.top = pt.y;
	rect.right = rect.left + (info.rcImage.right - info.rcImage.left);
	rect.bottom = rect.top + (info.rcImage.bottom - info.rcImage.top);
	pDC->RestoreDC(nSaveDC);
}

void		CEHomeList::Draw_ProgressItem(CDC* pDC, RECT rect, int nProg, int nColor)
{
	static COLORREF		colList[7]		= {RGB(0x68, 0xC0, 0x0A), RGB(0x65, 0x0B, 0xC3)
		, RGB(0xC4, 0x22, 0x17), RGB(0xC2, 0x0B, 0xB1), RGB(0xBD, 0x85, 0x0A)
		, RGB(0x05, 0x22, 0xBE), RGB(0x0A, 0xB0, 0xBE)};
	COLORREF			colBk			= RGB(0xDA, 0xDA, 0xDA);
	RECT				rt;
	TCHAR				szText[64]		= {0};

	rect.left += 10;
	rect.right -= 10;
	rect.top = rect.top + (rect.bottom - rect.top - 20) / 2;
	rect.bottom = rect.top + 20;
	pDC->FillSolidRect(&rect, colBk);
	rect.left += 2;
	rect.right -= 2;
	rect.top += 2;
	rect.bottom -= 2;
	rt = rect;
	rt.right = rect.left + (rect.right - rect.left) * nProg / 100;
	pDC->FillSolidRect(&rt, colList[nColor % 7]);
	rect.right -= 20;
	_itot(nProg, szText, 10);
	_tcscat(szText, _T("%"));
	pDC->DrawText(szText, _tcslen(szText), &rect, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
}

void		CEHomeList::Draw_LinkItem(CDC* pDC, RECT& rect, LPCTSTR lpLink, BOOL bMouseOver)
{
	RECT			rt;
	CFont*			pOldFont;
	COLORREF		colOld;

	if(NULL == m_FontLink.GetSafeHandle())
	{
		LOGFONT			logFont			= {0};

		GetFont()->GetLogFont(&logFont);
		logFont.lfUnderline = TRUE;
		m_FontLink.CreateFontIndirect(&logFont);
	}
	pOldFont = pDC->SelectObject(&m_FontLink);
	pDC->DrawText(lpLink, _tcslen(lpLink), &rt, DT_CALCRECT);
	rect.top = rect.top + ((rect.bottom - rect.top) - (rt.bottom - rt.top)) / 2; 
	rect.bottom = rect.top + (rect.bottom - rect.top);
	rect.right = min(rect.right, rect.left + (rt.right - rt.left));
	colOld = pDC->GetTextColor();
	if(bMouseOver)
		pDC->SetTextColor(RGB(0xFF, 0x0, 0x0));
	else
		pDC->SetTextColor(RGB(0x0, 0x0, 0xFF));
	pDC->DrawText(lpLink, _tcslen(lpLink), &rect, DT_SINGLELINE | DT_TOP | DT_WORD_ELLIPSIS);
	pDC->SetTextColor(colOld);
	pDC->SelectObject(pOldFont);
}
// 绘制图标列
void		CEHomeList::Draw_IconListItem(CDC* pDC, HIMAGELIST hImageList, RECT rect, int nIndex)
{
	if(NULL == hImageList)
	{
		ASSERT(NULL != hImageList);
		return;
	}

	IMAGEINFO			info		= {0};
	POINT				pt			= {0};
	int					nSaveDC		= pDC->SaveDC();
	CRgn				rgn;

	rgn.CreateRectRgnIndirect(&rect);
	pDC->SelectObject(rgn);
	ImageList_GetImageInfo(hImageList, 0, &info);
	pt.x = rect.left + ((rect.right - rect.left) - (info.rcImage.right - info.rcImage.left)) / 2;
	pt.y = rect.top + ((rect.bottom - rect.top) - (info.rcImage.bottom - info.rcImage.top)) / 2;
	ImageList_Draw(hImageList, nIndex, pDC->GetSafeHdc(), pt.x, pt.y, ILD_TRANSPARENT);
	pDC->RestoreDC(nSaveDC);
}
// 绘制多按钮
void CEHomeList::Draw_MuiltButtonItem(CDC* pDC, RECT& rect, HIMAGELIST hImageList, DWORD dwMask, int nIndex)
{
	if(NULL == hImageList)
		return;

	int				nSaveDC			= pDC->SaveDC();
	int				nCount			= ImageList_GetImageCount(hImageList) / 4;
	IMAGEINFO		imgInfo			= {0};
	CRgn			rgn;
	int				nImgWidth		= 0;
	int				nImgHeight		= 0;
	int				nLeft			= 0;
	RECT			rtSave			= rect;

	rgn.CreateRectRgnIndirect(&rect);
	pDC->SelectObject(&rgn);
	ImageList_GetImageInfo(hImageList, 0, &imgInfo);
	nImgWidth = imgInfo.rcImage.right - imgInfo.rcImage.left;
	nImgHeight = imgInfo.rcImage.bottom - imgInfo.rcImage.top;
	rect.left = rect.left + ( (rect.right - rect.left) - (nImgWidth * nCount + 5 * max(0, (nCount - 1))) ) / 2;
	rect.right = rect.left + (nImgWidth * nCount + 5 * max(0, (nCount - 1)));
	rect.top = rect.top + ( (rect.bottom - rect.top) - nImgHeight ) / 2;
	rect.bottom = rect.top + nImgHeight;
	// 开始绘制每一项
	nLeft = rect.left;

	for(int i = 0; i < nCount; i++, nLeft += nImgWidth + 5)
	{
		int		ni		= i * 4;

		if(0 == ((0x1 << i) & dwMask))
			ni += 3;
		else if(i == nIndex)
			ni += 1;
		ImageList_Draw(hImageList, ni, pDC->GetSafeHdc(), nLeft, rect.top, ILD_TRANSPARENT);
	}
	
	pDC->RestoreDC(nSaveDC);
}
// 自己绘制每一项
void CEHomeList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int				nUseW		= lpDrawItemStruct->rcItem.left;
	int				nImage		= 0;
	CRect			rt			= lpDrawItemStruct->rcItem;
	CDC				dc;
	CImageList*		pImage		= m_pImageList[LVSIL_SMALL];
	CPen*			pOldPen		= NULL;
	
	if(NULL == m_PenGridlines.GetSafeHandle())
	{
		m_PenGridlines.CreatePen(PS_SOLID, 1, RGB(0xe5,0xe5, 0xe5));
	}
	dc.Attach(lpDrawItemStruct->hDC);
	dc.SetBkMode(TRANSPARENT);
	pOldPen = dc.SelectObject(&m_PenGridlines);
	dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(0xff, 0xff, 0xff));
	m_vctData[lpDrawItemStruct->itemID].mapMouse.clear();
	for(int i = 0; i < (int)m_vctData[lpDrawItemStruct->itemID].vctstr.size(); i++)
	{
		LVCOLUMN	column		= {0};

		column.mask = LVCF_WIDTH | LVCF_FMT;
		GetColumn(i, &column);
		rt = lpDrawItemStruct->rcItem;
		rt.MoveToX(nUseW);
		rt.right = rt.left + column.cx;
		nUseW += column.cx;

		if(0 == i)
		{
			// 绘制图标
			if(NULL != pImage)
			{
				POINT		pt		= {0, 0};
				IMAGEINFO	info	= {0};

				pImage->GetImageInfo(0, &info);
				pt.x = rt.left+2;
				pt.y = rt.top + (rt.Height() - (info.rcImage.bottom - info.rcImage.top)) / 2;
				pImage->Draw(&dc, m_vctData[lpDrawItemStruct->itemID].nImage, pt, ILD_TRANSPARENT);
				nImage = info.rcImage.right - info.rcImage.left;
			}
			rt.left += nImage+4;
		}
		rt.DeflateRect(1, 1, 1, 1);
		// 初始化列
		if(m_vctColumnFmt.size() < m_ctrlHeader.GetItemCount())
		{
			EHOMELISTCOLUMN			column;

			column.fmt = EHLF_Normal;
			column.hImgList = NULL;
			m_vctColumnFmt.resize(m_ctrlHeader.GetItemCount(), column);
		}
		// 绘制项目
		switch(m_vctColumnFmt[i].fmt)
		{
		case EHLF_Title:	// 标题项绘制
			Draw_TitleItem(&dc, rt, m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str());
			break;
		case EHLF_SwitchButton:	// 交换按钮绘制
			{
				RECT		rtButton	= rt;

				Draw_SwitchButtonItem(&dc, rtButton, _ttoi(m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str())
					, m_vctColumnFmt[i].hImgList, m_dwMouseItem == MAKELONG(lpDrawItemStruct->itemID, i));
				m_vctData[lpDrawItemStruct->itemID].mapMouse[i] = rtButton;
			}
			break;
		case EHLF_Link: // 超连接绘制
			{
				RECT		rtLink		= rt;

				Draw_LinkItem(&dc, rtLink, m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str()
					, m_dwMouseItem == MAKELONG(lpDrawItemStruct->itemID, i));
				m_vctData[lpDrawItemStruct->itemID].mapMouse[i] = rtLink;
			}
			break;
		case EHLF_Progress:		// 进度条绘制
			Draw_ProgressItem(&dc, rt, _ttoi(m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str())
				, lpDrawItemStruct->itemID );
			break;
		case EHLF_IconList:		// 绘制图标列表
			Draw_IconListItem(&dc, m_vctColumnFmt[i].hImgList, rt
				, _ttoi(m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str()));
			break;
		case EHLF_MuiltButton:	// 绘制多按钮
			{
				RECT		rtButton		= rt;

				Draw_MuiltButtonItem(&dc, rtButton, m_vctColumnFmt[i].hImgList
					, _ttol(m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str())
					, (LOWORD(m_dwMouseItem) == lpDrawItemStruct->itemID)?m_dwMuiltIndex:-1);
				m_vctData[lpDrawItemStruct->itemID].mapMouse[i] = rtButton;
			}
			break;
		default: // 默认绘制
			Draw_NormalItem(&dc, rt, m_vctData[lpDrawItemStruct->itemID].vctstr[i].c_str(), column.fmt);
			break;
		}
		// 绘制边框
		dc.MoveTo(rt.left-1, rt.bottom);
		dc.LineTo(rt.right, rt.bottom);
		dc.LineTo(rt.right, rt.top-2);
	}

	dc.SelectObject(pOldPen);
	dc.Detach();
}
// 设置行高
void CEHomeList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItem)
{
	lpMeasureItem->itemHeight = m_nItemHeight;
}
// 鼠标移动
void CEHomeList::OnMouseMove(UINT nFlags, CPoint point)
{
	// 注册鼠标离开事件
	if(FALSE == m_bTrackMouseEvent)
	{
		TRACKMOUSEEVENT		track		= {0};

		track.cbSize = sizeof(TRACKMOUSEEVENT);
		track.dwHoverTime = 1;
		track.dwFlags = TME_LEAVE;
		track.hwndTrack = GetSafeHwnd();

		m_bTrackMouseEvent = TrackMouseEvent(&track);
	}

	LVHITTESTINFO			info		= {0};
	CRect					rtDraw1;
	DWORD					dwOld		= m_dwMouseItem;
	DWORD					dwNew		= -1;
	DWORD					dwMuilt		= m_dwMuiltIndex;

	info.pt = point;
	m_dwMuiltIndex = -1;
	if(-1 != SubItemHitTest(&info))
	{
		if(m_vctData[info.iItem].mapMouse.end() != m_vctData[info.iItem].mapMouse.find(info.iSubItem))
		{
			CRect		rect		= m_vctData[info.iItem].mapMouse[info.iSubItem];
			CRect		rtItem;
			
			// 查看是否在按钮区域内
 			if(0 == info.iSubItem)
 			{
 				rtItem = rect;
 			}
 			else
			{
				// 需要重新计算按钮的位置
				GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rtItem);
				//rtItem.left = rtItem.left + (rtItem.Width() - rect.Width()) / 2;
				rtItem.top = rtItem.top + (rtItem.Height() - rect.Height()) / 2;
				//rtItem.right = rtItem.left + rect.Width();
				rtItem.bottom = rtItem.top + rect.Height();
			}
			if(rtItem.PtInRect(point))
			{
				dwNew = MAKELONG(info.iItem, info.iSubItem);
				rtDraw1 = rtItem;
				if(EHLF_MuiltButton == m_vctColumnFmt[info.iSubItem].fmt)
				{
					m_dwMuiltIndex = MuiltButtonHitTest(point, rtItem, m_vctColumnFmt[info.iSubItem].hImgList);
				}
			}
		}
	}

	if(dwOld != dwNew)
	{
		m_dwMouseItem = dwNew;
		if(-1 != dwNew)
		{
			InvalidateRect(&rtDraw1, TRUE);
		}	
		if(-1 != dwOld)
		{
			rtDraw1 = m_vctData[LOWORD(dwOld)].mapMouse[HIWORD(dwOld)];
			InvalidateRect(&rtDraw1, TRUE);
		}
	}
	else if(dwMuilt != m_dwMuiltIndex)
	{
		InvalidateRect(&rtDraw1, FALSE);
	}
	// 按需显示手型
	if(-1 != dwNew && EHLF_Link == m_vctColumnFmt[HIWORD(dwNew)].fmt)
	{
		::SetCursor(m_hCursor[1]);
	}
	else
	{
		::SetCursor(m_hCursor[0]);
	}

	CListCtrl::OnMouseMove(nFlags, point);
}
// 多按钮样式
int CEHomeList::MuiltButtonHitTest(POINT pt, CRect &rect, HIMAGELIST hImage)
{
	int				nCount		= ImageList_GetImageCount(hImage) / 4;
	IMAGEINFO		imgInfo		= {0};
	CRect			rtItem;

	rtItem = rect;
	ImageList_GetImageInfo(hImage, 0, &imgInfo);
	for(int i = 0; i < nCount; i++)
	{
		rtItem.left += i * (imgInfo.rcImage.right - imgInfo.rcImage.left);
		rtItem.right = rtItem.left + (imgInfo.rcImage.right - imgInfo.rcImage.left);
		if(rtItem.PtInRect(pt))
			return i;
		rtItem.left += 5; // 间宽
	}
	return -1;
}

