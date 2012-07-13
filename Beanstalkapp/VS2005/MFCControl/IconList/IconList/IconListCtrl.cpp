#include "IconListCtrl.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
CILItem::CILItem(Gdiplus::Image* pImage, Gdiplus::Image* pImageEt, 
				 CSimpleDUIBase* parent, HICON hIcon, LPCWSTR lpText, UINT uID)
: CSimpleDUIBase(parent)
, m_image(pImage)
, m_uID(uID)
{
	RECT		rect;

	assert(NULL != pImage);
	m_status = 0;
	m_icon = Gdiplus::Bitmap::FromHICON(hIcon);
	memset(m_szText, 0, sizeof(m_szText));
	wcsncpy(m_szText, lpText, min(32, wcslen(lpText)));
	m_et = new CImageDUIButton(pImageEt, this, -uID);
	assert(NULL != m_et);
	rect.right = pImage->GetWidth() / 2;
	rect.top = 0;
	rect.left = rect.right - pImageEt->GetWidth() / 4;
	rect.bottom = pImageEt->GetHeight();
	m_et->ShowUI(FALSE);
	m_et->MoveUI(&rect);
}

CILItem::~CILItem()
{

}

BOOL CILItem::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_et->IsVisible())
	{
		return FALSE; // 编辑模式，不接收这些消息
	}

	RECT		rect;

	GetUIRect(&rect);
	if(WM_MOUSEMOVE == nMsg)
	{
		if(0 == m_status)
		{
			m_status = 1;
			UIInvalidate(&rect);
			TrackEvent(WM_MOUSELEAVE);
		}
	}
	else if(WM_MOUSELEAVE == nMsg)
	{
		m_status = 0;
		UIInvalidate(&rect);
	}
	else if(WM_LBUTTONDOWN == nMsg)
	{
		m_status = 1;
		TrackEvent(0);
		SetUICapture();
		UIInvalidate(&rect);
	}
	else if(WM_LBUTTONUP == nMsg)
	{
		POINT		pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

		if(FALSE == ReleaseUICapture())
		{
			return FALSE;
		}

		if(PtInRect(&rect, pt))
		{
			m_status = 1;
			TrackEvent(WM_MOUSELEAVE);
			SendMessage(GetUIRoot()->hWnd, WM_COMMAND, m_uID, 0);
		}
		else
		{
			m_status = 0;
		}
		UIInvalidate(&rect);
	}

	return TRUE;
}

void CILItem::OnUIDraw(HDC hDC, LPRECT lpRect)
{
	RECT				rect;
	Gdiplus::Graphics	graphic(hDC);
	int					nWidth		= m_image->GetWidth() / 2;
	int					nHeight		= m_image->GetHeight();
	HDC					hDC2;
	RECT				rtText;
	int					nIndex		= (m_status % 2);

	GetUIRect(&rect);
	MergerRect(&rect, &rect, lpRect);
	
	if(m_et->IsVisible())
	{
		nIndex = 1;		// 编辑模式
	}
	// 绘制背景
	graphic.DrawImage(m_image, Gdiplus::Rect(rect.left, rect.top, nWidth, nHeight),
		nWidth * nIndex, 0, nWidth, nHeight, Gdiplus::UnitPixel);
	// 绘制图标
	if(NULL != m_icon)
	{
		graphic.DrawImage(m_icon, Gdiplus::Rect(rect.left + 16, rect.top + 5, 48, 48),
			0, 0, m_icon->GetWidth(), m_icon->GetHeight(), Gdiplus::UnitPixel);
	}
	// 绘制文本
	hDC2 = graphic.GetHDC();
	rtText = rect;
	rtText.top = rtText.bottom - 32;
	::DrawText(hDC2, m_szText, wcslen(m_szText), &rtText, 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	graphic.ReleaseHDC(hDC2);
}

void CILItem::SetEditMode(BOOL bEtMode)
{
	m_et->ShowUI(bEtMode);
}

//////////////////////////////////////////////////////////////////////////
CILContainer::CILContainer()
{
	m_nCurrentGroup = -1;
	m_nSwitchGroup = 0;
	m_navigatebar = new CSimpleDUIPanel(this, RGB(0xff, 0xff, 0), 0);
	m_imageRadio = NULL;
	m_radiogroup = NULL;
	m_imageItemBk = NULL;
	m_imageItemEt = NULL;
	m_uNotifyMsg = 0;
	m_hNotifyWnd = NULL;
}

CILContainer::~CILContainer()
{
	if(NULL != m_imageRadio)
	{
		delete m_imageRadio;
	}
}

void CILContainer::OnUIDraw(HDC hDC, LPRECT lpRect)
{

}

BOOL CILContainer::TranslateUIEvent(HWND hWnd, UINT nMsg,
									WPARAM wParam, LPARAM lParam)
{
	if(WM_COMMAND == nMsg)
	{
		if(wParam < 500)
		{
			// 内部消息
			size_t		n		= wParam - 400;

			if(n >= m_groups.size())
			{
				return TRUE;
			}

			ShowGroup(n);
			return TRUE;
		}
		else
		{
			// 项消息
			if(0x80000 & wParam)
			{
				// 编辑模式消息
				PostMessage(m_hNotifyWnd, m_uNotifyMsg, -wParam - 500
					, MAKELPARAM(1, m_nCurrentGroup));
			}
			else
			{
				// 正常模式消息
				PostMessage(m_hNotifyWnd, m_uNotifyMsg, wParam - 500
					, MAKELPARAM(0, m_nCurrentGroup));
			}
		}
	}

	return CSimpleDUIRoot::TranslateUIEvent(hWnd, nMsg, wParam, lParam);
}

void CILContainer::SetGroupCount(size_t nCount)
{
	if(nCount > 5 || 0 == nCount)
	{
		assert(false);
		return;
	}

	// 少于原来的数
	if(m_groups.size() > nCount)
	{
		for(size_t i = nCount; i < m_groups.size(); i++)
		{
			delete m_groups[i];
			delete m_navigates[i];
		}

		m_groups.resize(nCount);
		m_navigates.resize(nCount);
		return;
	}
	else if(m_groups.size() < nCount)
	{
		for(size_t i = m_groups.size(); i < nCount; i++)
		{
			m_groups.push_back( new CSimpleDUIPanel(this, 0, 0) );
			m_navigates.push_back( new CImageDUIRadio(m_imageRadio, m_navigatebar, 
				400 + m_groups.size() - 1, &m_radiogroup) );
		}
	}

	// 清空原m_groups的子项
	for(size_t i = 0; i < m_groups.size(); i++)
	{
		while(NULL != m_groups[i]->GetChildUI())
		{
			delete m_groups[i]->GetChildUI();
		}
	}

	ShowGroup(0);
	UpdateNaviageBar();
}

size_t CILContainer::GetGroupCount()
{
	return m_groups.size();
}

BOOL CILContainer::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL CILContainer::AddItem(int nGroup, HICON hIcon, LPCWSTR lpText, UINT uID)
{
	if(nGroup < 0 || nGroup >=m_groups.size())
	{
		return FALSE;
	}

	CILItem*		pItem;

	pItem = new CILItem(m_imageItemBk, m_imageItemEt, m_groups[nGroup], 
		hIcon, lpText, uID + 500);
	
	return NULL != pItem;
}

BOOL CILContainer::DeleteItem(int nGroup, UINT uID)
{
	return FALSE;
}
/*
 *	调整导航栏位置
 */
BOOL CILContainer::UpdateNaviageBar()
{
	RECT		rtBar, rtNv;
	int			nWidth		= m_imageRadio->GetWidth() / 4;
	int			nHeight		= m_imageRadio->GetHeight();
	int			nSpace		= 10;
	int			nLeft		= 0;
	int			nTop		= 0;

	m_navigatebar->GetUIRect(&rtBar);
	nLeft = ((rtBar.right - rtBar.left) - (nWidth + nSpace) * m_navigates.size()
		- nSpace) / 2;
	nTop = (rtBar.bottom - rtBar.top - nHeight) / 2;
	for(size_t i = 0; i < m_navigates.size(); i++)
	{
		rtNv.left = nLeft;
		rtNv.top = nTop;
		rtNv.right = rtNv.left + nWidth;
		rtNv.bottom = rtNv.top + nHeight;
		nLeft += nWidth + nSpace;

		m_navigates[i]->MoveUI(&rtNv, FALSE);
	}

	m_navigatebar->ForegroundUI();

	return TRUE;
}
/*
 *	更新组
 */
BOOL CILContainer::UpdateGroup(int nGroup)
{
	RECT				rect;
	int					nLeft;
	int					nTop;
	int					nSpace		= 5;
	CSimpleDUIBase*		node;
	RECT				rtItem;
	int					nWidth		= m_imageItemBk->GetWidth() / 2;
	int					nHeight		= m_imageItemBk->GetHeight();

	if(nGroup < 0 || nGroup >= m_groups.size())
	{
		return FALSE;
	}

	GetUIRect(&rect);
	nLeft = rect.left + 5;
	nTop = rect.top + 5;
	for(node = m_groups[nGroup]->GetChildUI();
		NULL != node;
		node = node->GetBrotherUI())
	{
		if((nLeft + nSpace + nWidth) > rect.right)
		{
			nLeft = rect.left + 5;
			nTop += nHeight + nSpace;
		}

		rtItem.left = nLeft;
		rtItem.right = rtItem.left + nWidth;
		rtItem.top = nTop;
		rtItem.bottom = rtItem.top + nHeight;
		nLeft += nWidth + nSpace;
		node->MoveUI(&rtItem, FALSE);
	}

	UIInvalidate(NULL);
	return TRUE;
}

/*
 *	设置编辑模式
 */
void CILContainer::SetEditMode(BOOL bEtMode)
{
	for(size_t i = 0; i < m_groups.size(); i++)
	{
		CILItem*		pItem		= (CILItem *)m_groups[i]->GetChildUI();

		for(; NULL != pItem; pItem = (CILItem*)pItem->GetBrotherUI())
		{
			pItem->SetEditMode(bEtMode);
		}
	}

	UIInvalidate(NULL);
}

BOOL CILContainer::ShowGroup(int nIndex)
{
	if(m_groups.size() <= nIndex || nIndex < 0
		|| nIndex == m_nCurrentGroup)
	{
		return FALSE;
	}
	m_nCurrentGroup = nIndex;
	for(size_t i = 0; i < m_groups.size(); i++)
	{
		if(i == nIndex)
		{
			m_groups[i]->ShowUI(TRUE);
		}
		else
		{
			m_groups[i]->ShowUI(FALSE);
		}
	}
	RECT		rect;

	GetUIRect(&rect);
	m_groups[nIndex]->MoveUI(&rect);
	m_radiogroup = m_navigates[m_nCurrentGroup];

	return TRUE;
}

BOOL CILContainer::AnimationShowGroup(int nIndex)
{
	if(m_groups.size() <= nIndex || nIndex < 0)
	{
		return FALSE;
	}

	m_nSwitchGroup = nIndex;
	::SetTimer(GetUIRoot()->hWnd, (ULONG_PTR)this, 100, NULL);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
CIconListCtrl::CIconListCtrl()
{
// 	m_text = NULL;
// 	m_text2 = NULL;
// 	m_text3 = NULL;
}

CIconListCtrl::~CIconListCtrl()
{

}
/*
 *	消息转发
 */
BOOL CIconListCtrl::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, 
										 LPARAM lParam, LRESULT& lResult, 
										 DWORD dwMsgMapID)
{
	if(hWnd != m_hWnd)
	{
		return FALSE;
	}

	if( FALSE != TranslateUIEvent(hWnd, uMsg, wParam, lParam) )
	{
		return FALSE;
	}

	return FALSE;
}
/*
 *	附加窗体
 */
BOOL CIconListCtrl::SubclassWindow(HWND hWnd)
{
	if(FALSE == __super::SubclassWindow(hWnd))
	{
		return FALSE;
	}

	RECT	rect;

	GetClientRect(&rect);
	MoveUI(&rect);
	rect.top = rect.bottom - 60;
	rect.bottom -= 10;
	m_navigatebar->MoveUI(&rect);

	// 添加子项
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

