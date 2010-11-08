// TreeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "TreeListCtrl.h"
#include ".\treelistctrl.h"

// |space|checkbox|ckspace|degree*indent|button|btnspace|icon|iconspace|text|
#define TREELIST_COLUMN_SPACE		4
#define TREELIST_COLUMN_BUTTON		9
#define TREELIST_COLUMN_BTNSPACE	4
#define TREELIST_COLUMN_INDENT		16
#define TREELIST_COLUMN_CKSPACE		5
#define TREELIST_COLUMN_CHECKBOX	16
#define TREELIST_COLUMN_ICONSPACE	3

//////////////////////////////////////////////////////////////////////////
// CTreeList
CTreeList::CTreeList()
{
	m_root.parent = NULL;
	m_root.bExpend = TRUE;
}

CTreeList::~CTreeList()
{
	DeleteAllItems();
}
// 得到迭代器
TreeListPtr CTreeList::GetItemPtr(HTREELIST hParent, HTREELIST hItem)
{
	TreeListPtr			ptr;

	for(ptr = hParent->childs.begin()
		; ptr != hParent->childs.end()
		; ptr++)
	{
		if(*ptr == hItem)
			break;
	}

	return ptr;
}

// 插入数据
HTREELIST CTreeList::InsertItem(LPCTSTR lpText, int nImage, HTREELIST hParnet /* = NULL */, HTREELIST hAfter /* = NULL */)
{
	HTREELIST		hItem			= new TREELISTNODE;

	hItem->bExpend = FALSE;
	hItem->nImage = nImage;
	hItem->nState = 1;
	hItem->lParam = NULL;
	hItem->vTexts.push_back(lpText);
	
	// 设置父标记
	if(NULL == hParnet)
	{
		hParnet = &m_root;
	}
	hItem->parent = hParnet;
	// 设置插入点
	if(NULL == hAfter)
	{
		hParnet->childs.push_back(hItem);
	}
	else
	{
		hParnet->childs.insert(GetItemPtr(hParnet, hAfter), hItem);
	}
	
	return hItem;
}

// 设置文本
void CTreeList::SetItemText(HTREELIST hItem, int nSubItem, LPCTSTR lpText)
{
	if(NULL == hItem)
		return ;

	if(hItem->vTexts.size() < (nSubItem + 1))
		hItem->vTexts.resize(nSubItem+1);

	hItem->vTexts[nSubItem] = lpText;
}

void CTreeList::SetItemData(HTREELIST hItem, DWORD_PTR pData)
{
	if(NULL == hItem)
		return ;

	hItem->lParam = (LPARAM)pData;
}

void CTreeList::SetItemState(HTREELIST hItem, int nState)
{
	if(NULL == hItem)
		return;

	hItem->nState = nState;
}

void CTreeList::SetItemImage(HTREELIST hItem, int nImage)
{
	if(NULL == hItem)
		return;

	hItem->nImage = nImage;
}

void CTreeList::GetItemText(HTREELIST hItem, int nSubItem, LPTSTR lpText, int nMaxLen)
{
	lpText[0] = 0;

	if(NULL == hItem)
		return;
	if(hItem->vTexts.size() < (nSubItem + 1))
		return;

	lstrcpyn(lpText, hItem->vTexts[nSubItem].c_str(), nMaxLen);
}

DWORD_PTR CTreeList::GetItemData(HTREELIST hItem)
{
	if(NULL == hItem)
		return NULL;

	return (DWORD_PTR)hItem->lParam;
}

int CTreeList::GetItemState(HTREELIST hItem)
{
	if(NULL == hItem)
		return 0;

	return hItem->nState;
}

int CTreeList::GetItemImage(HTREELIST hItem)
{
	if(NULL == hItem)
		return 0;

	return hItem->nImage;
}
// 删除子节点
void CTreeList::DeleteChildItems(HTREELIST hParent)
{
	if(NULL == hParent)
		return;

	for(int i = 0; i < hParent->childs.size(); i++)
	{
		DeleteChildItems(hParent->childs[i]);
		delete hParent->childs[i];
	}

	hParent->childs.clear();
}

// 删除节点
void CTreeList::DeleteItem(HTREELIST hItem)
{
	if(NULL == hItem)
		return;

	// delete child node
	DeleteChildItems(hItem);

	// delete self
	if(NULL == hItem->parent)
		return;

	HTREELIST		hParent;

	hParent = hItem->parent;
	hParent->childs.erase(GetItemPtr(hParent, hItem));

	delete hItem;
}

// 删除所有节点
void CTreeList::DeleteAllItems()
{
	DeleteChildItems(&m_root);
}

// 得到子节点可见项
void CTreeList::GetChildVisibleItems(std::vector<HTREELIST>& vItems, HTREELIST hParent)
{
	if(NULL == hParent)
		return;
	if(FALSE == hParent->bExpend)
		return;

	for(int i = 0; i < hParent->childs.size(); i++)
	{
		vItems.push_back(hParent->childs[i]);
		GetChildVisibleItems(vItems, hParent->childs[i]);
	}
}

// 得到可见节点
void CTreeList::GetVisibleItems(std::vector<HTREELIST>& vItems)
{
	vItems.clear();

	GetChildVisibleItems(vItems, &m_root);
}

// 展开节点 
void CTreeList::Expand(HTREELIST hItem, UINT nCode)
{
	if(NULL == hItem)
		return;

	if(NULL == hItem->parent)
	{
		hItem->bExpend = TRUE;
		return;
	}

	switch(nCode)
	{
	case TVE_COLLAPSE:
		hItem->bExpend = FALSE;
		break;
	case TVE_EXPAND:
		hItem->bExpend = TRUE;
		break;
	case TVE_TOGGLE:
		hItem->bExpend = !hItem->bExpend;
		break;
	}
}

HTREELIST CTreeList::GetParentItem(HTREELIST hItem)
{
	if(NULL == hItem)
		return NULL;
	
	return hItem->parent;
}
BOOL CTreeList::ItemHasChildren(HTREELIST hItem)
{
	if(NULL == hItem)
		return FALSE;

	return (hItem->childs.size() != 0);
}
int CTreeList::GetChildCount(HTREELIST hParent)
{
	int		nCount		= 0;

	if(NULL == hParent)
		return 0;

	nCount = hParent->childs.size();
	for(int i = 0; i < hParent->childs.size(); i++)
	{
		nCount += GetChildCount(hParent->childs[i]);
	}

	return nCount;
}
int CTreeList::GetCount()
{
	return GetChildCount(&m_root);
}
// 得到路径深度
int CTreeList::GetTreeDegree(HTREELIST hItem)
{
	if(NULL == hItem)
		return -1;

	int			nDegree = 0;

	for(HTREELIST hParent = hItem->parent
		; NULL != hParent
		; hParent = hParent->parent)
	{
		nDegree++;
	}
	
	return nDegree;
}
// 得到这个支点的顶端节点
HTREELIST CTreeList::GetTopItem(HTREELIST hItem)
{
	HTREELIST		hTopItem	= hItem;

	for(HTREELIST hParent = hItem->parent
		; (NULL != hParent) && (NULL != hParent->parent)
		; hParent = hParent->parent)
	{
		hTopItem = hParent;
	}

	return hTopItem;
}
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTreeListCtrl, CListCtrl)
CTreeListCtrl::CTreeListCtrl()
{
	m_bDataChange = FALSE;
	VERIFY(m_pData = new CTreeList());
	m_pOldData = m_pData;
}

CTreeListCtrl::~CTreeListCtrl()
{
	delete m_pData;
}


BEGIN_MESSAGE_MAP(CTreeListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// 初始化
void CTreeListCtrl::InitTreeListCtrl()
{
	DWORD			dwStyle;

	// 控件风格
	dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	ASSERT(dwStyle & LVS_OWNERDATA);
	dwStyle |= LVS_REPORT;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);

	// 扩展风格
// 	dwStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
// 	dwStyle |= LVS_EX_FULLROWSELECT;
// 	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwStyle);
	dwStyle = LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/ | LVS_EX_CHECKBOXES;
	SetExtendedStyle(dwStyle);
}

void CTreeListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	InitTreeListCtrl();
}

// 设置
HTREELIST CTreeListCtrl::InsertItem(LPCTSTR lpText, int nImage, HTREELIST hParent /* = NULL */, HTREELIST hAfter /* = NULL */)
{
	HTREELIST		hItem;

	hItem = m_pData->InsertItem(lpText, nImage, hParent, hAfter);
	ASSERT(NULL != hItem);
	m_bDataChange = TRUE;

	return hItem;
}
// 设置文本
BOOL CTreeListCtrl::SetItemText(HTREELIST hItem, int nSubItem, LPCTSTR lpszText)
{
	m_pData->SetItemText(hItem, nSubItem, lpszText);
	m_bDataChange = TRUE;
	return TRUE;
}
// 设置数据项
BOOL CTreeListCtrl::SelectData(CTreeList* pData)
{
	if(NULL == pData)
		m_pData = m_pOldData;
	else
		m_pData = pData;
	
	UpdateData(TRUE);
	return TRUE;
}
// CheckAll
void CTreeListCtrl::CheckAll(BOOL bCheck /* = TRUE */)
{
	for(int i = 0; i < m_list.size(); i++)
	{
		HTREELIST		hItem		= m_list[i];

		if(1 != CTreeList::GetTreeDegree(hItem))
			continue;

		if(hItem->nState < 1 || hItem->nState > 2)
			continue;

		hItem->nState = bCheck?2:1;
		RedrawItems(i, i);
	}
}
// GetCheck
BOOL CTreeListCtrl::GetCheck(HTREELIST hItem)
{
	if(NULL == hItem)
		return FALSE;

	HTREELIST		hTop	= CTreeList::GetTopItem(hItem);

	return (2 == hTop->nState);
}
BOOL CTreeListCtrl::GetCheck(int nItem)
{
	if(nItem < 0 || nItem > m_list.size())
		return FALSE;

	return GetCheck(m_list[nItem]);
}
// SetCheck
void CTreeListCtrl::SetCheck(HTREELIST hItem, BOOL bCheck /* = TRUE */)
{
	if(NULL == hItem)
		return;

	HTREELIST		hTopItem	= CTreeList::GetTopItem(hItem);

	hTopItem->nState = bCheck?2:1;
	for(int i = 0; i < m_list.size(); i++)
	{
		if(hTopItem == m_list[i])
		{
			RedrawItems(i, i);
			break;
		}
	}
}
void CTreeListCtrl::SetCheck(int nItem, BOOL bCheck /* = TRUE */)
{
	if(nItem < 0 || nItem >m_list.size())
		return;

	return SetCheck(m_list[nItem], bCheck);
}
// 更新数据
void CTreeListCtrl::UpdateData(BOOL bForce /* = FALSE */)
{
	if(FALSE == m_bDataChange && FALSE == bForce)
		return;

	m_pData->GetVisibleItems(m_list);
	m_bDataChange = FALSE;

	CListCtrl::SetItemCountEx(m_list.size(), LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);
	CListCtrl::Invalidate();
}

// 虚拟表得到数据
void CTreeListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO*	pDispInfo		= reinterpret_cast<LV_DISPINFO*>(pNMHDR);
	LV_ITEM*		pItem			= &(pDispInfo)->item;

	int iItemIndx= pItem->iItem;
	if(pItem->mask & LVIF_TEXT)
	{
		if(0 != pItem->iSubItem)
			m_pData->GetItemText(m_list[iItemIndx], pItem->iSubItem, pItem->pszText, pItem->cchTextMax);
	}

	*pResult = 0;
}

void CTreeListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if(CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(CDDS_ITEMPREPAINT  == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pNMCD->nmcd.dwDrawStage)
	{
		if(0 == pNMCD->iSubItem)
		{
			DrawColumn_1(pNMCD);
			*pResult = CDRF_SKIPDEFAULT;
		}
	}
}

// 绘制第一列
void CTreeListCtrl::DrawColumn_1(LPNMLVCUSTOMDRAW pNMCD)
{
	CDC*		pDC;
	int			nItem			= pNMCD->nmcd.dwItemSpec;
	int			nSubItem		= pNMCD->iSubItem;
	CString		strText;
	CRect		rcItem;
	HTREELIST	hItem			= m_list[nItem];
	int			nDegree			= CTreeList::GetTreeDegree(hItem);

	pDC = CDC::FromHandle(pNMCD->nmcd.hdc);			// 获取绘画dc
	GetSubItemRect(nItem, 0, LVIR_BOUNDS, rcItem);
	rcItem.right = rcItem.left + GetColumnWidth(0);
	// 绘制背景
	DWORD	bkColor = 0;
	DWORD	TxtColor = 0;
	if( pNMCD->nmcd.uItemState & ODS_SELECTED )
	{
		bkColor = GetSysColor( COLOR_HIGHLIGHT );
		TxtColor = GetSysColor( COLOR_HIGHLIGHTTEXT );
	}
	else
	{
		bkColor = GetSysColor( COLOR_WINDOW );
		TxtColor = GetSysColor( COLOR_WINDOWTEXT );
	}
	pDC->FillSolidRect(&rcItem, bkColor);
// 	// 绘制边框
// 	if(GetItemState(nItem, LVIS_FOCUSED ) & LVIS_FOCUSED )
// 	{
// 		pDC->DrawFocusRect(&rcItem);
// 	}
	// |space|checkbox|ckspace|degree*indent|button|btnspace|icon|text|
	rcItem.left += TREELIST_COLUMN_SPACE;
	rcItem.left += (nDegree - 1) * TREELIST_COLUMN_INDENT;
	// 绘制CHECKBOX
	if((GetExtendedStyle() & LVS_EX_CHECKBOXES))
	{
		if(1 == nDegree)
			DrawItemCheckBox(pDC, rcItem, hItem->nState - 1);
		rcItem.left += TREELIST_COLUMN_CHECKBOX;
		rcItem.left += TREELIST_COLUMN_CKSPACE;
	}
	// 绘制BUTTON
	if(CTreeList::ItemHasChildren(hItem))
	{
		DrawItemButton(pDC, rcItem, hItem->bExpend);
	}
	rcItem.left += TREELIST_COLUMN_BUTTON;
	rcItem.left += TREELIST_COLUMN_BTNSPACE;
	// 绘制图标
	if(hItem->nImage >= 0)
	{
		rcItem.left += DrawItemIcon(pDC, rcItem, hItem->nImage);
	}
	// 绘制文本
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor( TxtColor );
 
	pDC->DrawText( hItem->vTexts[0].c_str(), hItem->vTexts[0].size()
		 		, &rcItem
				, DT_NOPREFIX | DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_VCENTER );
}

// 绘制BUGGON　返回值为BUTTON的宽度
int CTreeListCtrl::DrawItemButton(CDC* pDC, RECT rect, BOOL bExpand)
{
	int			nBtWidth		= 9;

	rect.top += ((rect.bottom - rect.top) - nBtWidth) / 2;
	rect.bottom = rect.top + nBtWidth;
	rect.right = rect.left + nBtWidth;
	pDC->FillSolidRect(&rect, RGB(0x80, 0x80, 0x80));
	::InflateRect(&rect, -1, -1);
	pDC->FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));
	::InflateRect(&rect, -1, -3);
	pDC->FillSolidRect(&rect, 0x0);
	// 没有展开状态
	if(FALSE == bExpand)
	{
		::InflateRect(&rect, -2, 2);
		pDC->FillSolidRect(&rect, 0x0);
	}
	
	return nBtWidth;
}

// 绘制CHECKBOX　返回值为CHECKBOX的宽度
int CTreeListCtrl::DrawItemCheckBox(CDC* pDC, RECT rect, int nImg)
{
	int					nCkWidth		= 16;
	int					nCkHeight		= 16;
	CImageList*			pImgState		= GetImageList(LVSIL_STATE);

	if(NULL == pImgState)
	{
		return 0; // 没有绘制图标
	}
	else
	{
		IMAGEINFO			info;
		POINT				pt;

		pImgState->GetImageInfo(0, &info);
		if(nImg < 0 || (pImgState->GetImageCount()-1) < nImg)
			return 0;	// 超出绘制图片的范围
		nCkWidth = info.rcImage.left - info.rcImage.right;
		nCkHeight = info.rcImage.bottom - info.rcImage.top;
		pt.x = rect.left;
		pt.y = rect.top + ((rect.bottom - rect.top) - nCkHeight) / 2;
		pImgState->Draw(pDC, nImg, pt, ILD_TRANSPARENT);
	}

	return nCkWidth;
}

// 绘制图片
int CTreeListCtrl::DrawItemIcon(CDC* pDC, RECT rect, int nImg)
{
	CImageList*			pImgIcon		= GetImageList(LVSIL_SMALL);
	IMAGEINFO			nInfo;
	int					nImgWidth		= 0;
	int					nImgHeight		= 0;
	POINT				pt;

	if(NULL == pImgIcon || pImgIcon->GetImageCount() == 0)
		return 0;

	pImgIcon->GetImageInfo(0, &nInfo);
	if(nImg < 0 || nImg > (pImgIcon->GetImageCount() - 1))
		return 0;	// 超出绘制范围

	nImgWidth = nInfo.rcImage.right - nInfo.rcImage.left;
	nImgHeight = nInfo.rcImage.bottom - nInfo.rcImage.top;
	pt.x = rect.left;
	pt.y = rect.top + ((rect.bottom - rect.top) - nImgHeight) / 2;
	pImgIcon->Draw(pDC, nImg, pt, ILD_TRANSPARENT);

	return nImgWidth + TREELIST_COLUMN_ICONSPACE;
}

// 双击展开数据
void CTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO		info		= {0};

	info.pt = point;
	SubItemHitTest(&info);

	if(-1 != info.iItem && 0 == info.iSubItem)
	{
		if(m_pData->ItemHasChildren(m_list[info.iItem]))
		{
			m_pData->Expand(m_list[info.iItem], TVE_TOGGLE);
			CListCtrl::OnLButtonDblClk(nFlags, point);
			UpdateData(TRUE);
			return ;
		}
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}
// 单击处理
void CTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO		info		= {0};

	info.pt = point;
	SubItemHitTest(&info);

	if(-1 != info.iItem && 0 == info.iSubItem)
	{
		CRect		rcCk(0, 0, 0, 0);
		CRect		rcBtn(0, 0, 0, 0);
		HTREELIST	hItem		= m_list[info.iItem ];

		GetItemCtrlRect(info.iItem , &rcCk, &rcBtn);
		// CHECKBOX
		if(!rcCk.IsRectEmpty() && rcCk.PtInRect(point) 
			&& (hItem->nState > 0 && hItem->nState < 3))
		{
			hItem->nState = 1 + hItem->nState % 2;
			RedrawItems(info.iItem, info.iItem);
		}
		else if(CTreeList::ItemHasChildren(hItem) && rcBtn.PtInRect(point))
		{
			m_pData->Expand(hItem, TVE_TOGGLE);
			CListCtrl::OnLButtonDown(nFlags, point);
			UpdateData(TRUE);
			return;
		}
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}

void CTreeListCtrl::GetItemCtrlRect(int nItem, LPRECT lpCheckBox, LPRECT lpButton)
{
	CRect		rect;
	HTREELIST	hItem		= m_list[nItem];
	int			nDegree		= CTreeList::GetTreeDegree(hItem);
	RECT&		rcCkBox		= *lpCheckBox;
	RECT&		rcBtn		= *lpButton;

	if(-1 == nItem)
		return ;

	GetItemRect(nItem, &rect, LVIR_BOUNDS);
	rcBtn.left = rect.left + TREELIST_COLUMN_SPACE;
	if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
	{
		if(1 == nDegree)
		{
			rcCkBox.left = rect.left + TREELIST_COLUMN_SPACE;
			rcCkBox.right = rcCkBox.left + TREELIST_COLUMN_CHECKBOX;
			rcCkBox.top = rect.top + (rect.Height() - TREELIST_COLUMN_CHECKBOX) / 2;
			rcCkBox.bottom = rcCkBox.top + TREELIST_COLUMN_CHECKBOX;
		}
		rcBtn.left += TREELIST_COLUMN_CHECKBOX + TREELIST_COLUMN_CKSPACE;
	}
	rcBtn.left += (nDegree - 1) * TREELIST_COLUMN_INDENT;
	rcBtn.right = rcBtn.left + TREELIST_COLUMN_BUTTON;
	rcBtn.top = rect.top + (rect.Height() - TREELIST_COLUMN_BUTTON) / 2;
	rcBtn.bottom = rect.top + TREELIST_COLUMN_BUTTON;
	::InflateRect(lpButton, 3, 3);
}