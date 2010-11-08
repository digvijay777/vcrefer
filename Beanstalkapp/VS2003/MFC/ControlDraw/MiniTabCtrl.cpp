// MiniTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "MiniTabCtrl.h"
#include ".\minitabctrl.h"


// CMiniTabCtrl
#define ITEM_SPACE		4
#define ITEM_BOTTOM		2

IMPLEMENT_DYNAMIC(CMiniTabCtrl, CWnd)
CMiniTabCtrl::CMiniTabCtrl()
{
	m_nCurSel = -1;
	m_color = GetSysColor(COLOR_MENUTEXT);
}

CMiniTabCtrl::~CMiniTabCtrl()
{
	if(NULL != m_brushItemNormal.m_hObject)
		m_brushItemNormal.DeleteObject();
	if(NULL != m_brushItemHover.m_hObject)
		m_brushItemHover.DeleteObject();
	if(NULL != m_bmpBk.m_hObject)
		m_bmpBk.DeleteObject();
	if(NULL != m_bmpItemNormal.m_hObject)
		m_bmpItemNormal.DeleteObject();
	if(NULL != m_bmpItemHover.m_hObject)
		m_bmpItemHover.DeleteObject();
	if(NULL != m_dcMem.m_hDC)
		m_dcMem.DeleteDC();

	// 释放空间
// 	std::vector<TCITEM>::iterator	tcItem	= m_vecTcitem.begin();
// 	for(int i = 1; tcItem != m_vecTcitem.end(); i++, tcItem++)
// 	{
// 		free(tcItem->pszText);
// 	}
}


BEGIN_MESSAGE_MAP(CMiniTabCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CMiniTabCtrl::InitTabCtrl(SIZE *siItem, UINT nIDNorBmp /* = -1 */, UINT nIDHovBmp /* = -1 */, UINT nIDBkBmp /* = -1 */)
{
	m_sizeItem = *siItem;
	ModifyStyle(0, SS_NOTIFY, 0);
	// 加载普通ITEM图片
	if(-1 != nIDNorBmp)
	{
		m_bmpItemNormal.LoadBitmap(nIDNorBmp);
		m_brushItemNormal.CreatePatternBrush(&m_bmpItemNormal);
	}
	else
	{
		m_brushItemNormal.CreateSolidBrush(GetSysColor(COLOR_MENU));
	}
	// 加载选中ITEM图片
	if(-1 != nIDHovBmp)
	{
		m_bmpItemHover.LoadBitmap(nIDHovBmp);
		m_brushItemHover.CreatePatternBrush(&m_bmpItemHover);
	}
	else
	{
		m_brushItemHover.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	}
	// 创建内存DC
	CDC*	pSelfDC = GetDC();
	m_dcMem.CreateCompatibleDC(GetDC());
	ReleaseDC(pSelfDC);
	// 加载背景
	if(-1 != nIDBkBmp)
	{
		m_bmpBk.LoadBitmap(nIDBkBmp);
		BITMAP	bit = {0};
		RECT	rect;

		m_bmpBk.GetBitmap(&bit);
		rect.left = 0;
		rect.top = 0;
		rect.bottom = bit.bmHeight;
		rect.right = bit.bmWidth;
		ClientToScreen(&rect);
		MoveWindow(&rect);
	}

	return TRUE;
}

LONG CMiniTabCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nImage /* = -1 */)
{
	if(-1 == nItem)
		return -1;

	std::vector<TCITEM>::iterator	tcItem	= m_vecTcitem.begin();
	for(int i = 1; tcItem != m_vecTcitem.end(); i++, tcItem++)
	{
		if(i > nItem)
			break;
	}
	TCITEM		NewTcitem = {0};

	NewTcitem.mask = TCIF_TEXT;
	NewTcitem.pszText = (LPTSTR)malloc(_tcslen(lpszItem) + 1);
	strcpy(NewTcitem.pszText, lpszItem);
    
	m_vecTcitem.insert(tcItem, NewTcitem);
	return nItem;
}

BOOL CMiniTabCtrl::SetCurSel(int nIndex)
{
	if((nIndex < 0) || (nIndex >= (int)m_vecTcitem.size()))
		return FALSE;
	m_nCurSel = nIndex;
	Invalidate();
	// 向父窗体发送消息
	NMHDR nm;
	nm.hwndFrom = m_hWnd;
	nm.idFrom = GetDlgCtrlID();
	nm.code = TCN_SELCHANGE;
	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&nm);
	return TRUE;
}
int CMiniTabCtrl::GetCurSel() const
{
	return m_nCurSel;
}
int CMiniTabCtrl::GetItemCount() const
{
	return (int)m_vecTcitem.size();
}
void CMiniTabCtrl::SetTextColor(COLORREF& color)
{
	m_color = color;
}
void CMiniTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect		rect;
	GetClientRect(&rect);
	
	// 得到透明背景
	if(NULL == m_bmpBk.m_hObject)
	{
		CDC*	pParentDC = GetParent()->GetDC();
		CRect	rtParent;

		GetWindowRect(&rtParent);
		GetParent()->ScreenToClient(&rtParent);
		m_bmpBk.CreateBitmap(rtParent.Width(), rtParent.Height(), 1, 32, NULL);
		CBitmap* pbmp = m_dcMem.SelectObject(&m_bmpBk);
		m_dcMem.BitBlt(0, 0, rect.Width(), rect.Height(), pParentDC, rect.left, rect.top, SRCCOPY);

		BOOL		bRet = TRUE;
		CRect		rtClient;
		CRect		rtSelf;

		GetParent()->GetClientRect(&rtClient);
		GetWindowRect(&rtSelf);
		GetParent()->ScreenToClient(&rtSelf);

		//dc.BitBlt(0, 0, rtSelf.Width(), rtSelf.Height(), GetParent()->GetDC(), rtSelf.left, rtSelf.top, SRCCOPY);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc, rtSelf.Width(), rtSelf.Height());
		CBitmap* pBmp = memDC.SelectObject(&bitmap);
		memDC.SetWindowOrg(rtSelf.left, rtSelf.top);
		GetParent()->SendMessage(WM_ERASEBKGND, (WPARAM)memDC.m_hDC, 0);
		memDC.SetViewportOrg(rtSelf.left, rtSelf.top);

		m_dcMem.BitBlt(0, 0, rtSelf.Width(), rtSelf.Height(), &memDC, 0, 0, SRCCOPY);

		m_dcMem.SelectObject(pbmp);
		ReleaseDC(pParentDC);

	}

	// 开始绘制
	CBitmap *pBmp = m_dcMem.SelectObject(&m_bmpBk);
	// 绘制项
	std::vector<TCITEM>::iterator	tcItem = m_vecTcitem.begin();
	for(int i = 0; tcItem != m_vecTcitem.end(); i++, tcItem++)
	{
		RECT	rtItem;
		
		rtItem.left = rect.left + i * (m_sizeItem.cx + ITEM_SPACE);
		rtItem.right = rtItem.left + m_sizeItem.cx;
		rtItem.bottom = rect.bottom - ITEM_BOTTOM;
		rtItem.top = rtItem.bottom - m_sizeItem.cy;

		if(i == m_nCurSel)
			m_dcMem.FillRect(&rtItem, &m_brushItemHover);
		else
			m_dcMem.FillRect(&rtItem, &m_brushItemNormal);

		rtItem.left += 2;
		rtItem.right -= 2;
		m_dcMem.SetBkMode(TRANSPARENT);
		m_dcMem.SetTextColor(m_color);
		m_dcMem.DrawText(tcItem->pszText, (int )strlen(tcItem->pszText), &rtItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcMem, 0, 0, SRCCOPY);
	m_dcMem.SelectObject(pBmp);
}

void CMiniTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 计算选中的项
	int		nXitem, nYitem, nXRel;
	RECT	rect;

	GetClientRect(&rect);
	nXitem = (point.x + m_sizeItem.cx + ITEM_SPACE - 1) / (m_sizeItem.cx + ITEM_SPACE);
	nXRel = point.x % (m_sizeItem.cx + ITEM_SPACE);
	nYitem = point.y;
	if((nXRel <= m_sizeItem.cx) &&
		(nYitem >= (rect.bottom - m_sizeItem.cy - ITEM_BOTTOM)) &&
		(nYitem <= (rect.bottom - ITEM_BOTTOM)))
	{
		SetCurSel(nXitem - 1);
	}
	CWnd::OnLButtonDown(nFlags, point);
}
