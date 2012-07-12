#include "IconListCtrl.h"

//////////////////////////////////////////////////////////////////////////
CILItem::CILItem(CSimpleDUIBase* parent)
: CSimpleDUIBase(parent)
{

}

CILItem::~CILItem()
{

}

//////////////////////////////////////////////////////////////////////////
CILContainer::CILContainer(CSimpleDUIBase* parent)
: CSimpleDUIBase(parent)
{
	m_nShowGroup = 0;
}

CILContainer::~CILContainer()
{

}

void CILContainer::OnUIDraw(HDC hDC, LPRECT lpRect)
{

}

BOOL CILContainer::AddGroup()
{
	size_t		st		= m_groups.size();

	if(5 <= st)
	{
		return FALSE;
	}

	m_groups.push_back(new CSimpleDUIPanel(this, 
		(st%2)?RGB(0xff, 0x0, 0x0):RGB(0x0, 0xff, 0x0)));

	ShowGroup(m_nShowGroup);
	return TRUE;
}

BOOL CILContainer::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if(WM_LBUTTONDOWN == nMsg)
	{
		if(m_groups.size() == 0)
		{
			return FALSE;
		}

		m_nShowGroup++;
		m_nShowGroup = m_nShowGroup % m_groups.size();
		ShowGroup(m_nShowGroup);
		return TRUE;
	}

	return FALSE;
}

BOOL CILContainer::DeleteGroup(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_groups.size())
	{
		return FALSE;
	}

	delete m_groups[nIndex];
	m_groups.erase(m_groups.begin() + nIndex);

	return TRUE;
}

BOOL CILContainer::AddItem(int nGroup, CILItem* item)
{
	return FALSE;
}

BOOL CILContainer::DeleteItem(int nGroup, CILItem* item)
{
	return FALSE;
}

BOOL CILContainer::ShowGroup(int nIndex)
{
	if(m_groups.size() <= nIndex || nIndex < 0)
	{
		return FALSE;
	}

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

// 	if(WM_COMMAND == uMsg)
// 	{
// 		if(100 == wParam)
// 		{
// 			m_button2->ShowUI(!m_button2->IsVisible());
// 		}
// 		else if(101 == wParam)
// 		{
// 			RECT		rect;
// 
// 			m_text2->GetUIRect(&rect);
// 			rect.left += 10;
// 			rect.right += 10;
// 			m_text2->MoveUI(&rect);
// 		}
// 	}
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
// 	RECT		rt	= {0, 0, 100, 30};
// 
// 	m_text = new CSimpleDUIText(L"Hello", this);
// 	m_text2 = new CSimpleDUIText(L"这是测试一个文本区域是否正常", this);
// 	m_text3 = new CSimpleDUIText(L"Hi", m_text2);
// 	m_text->MoveUI(&rt);
// 	rt.top += 5;
// 	rt.left += 10;
// 	rt.right += 100;
// 	m_text2->MoveUI(&rt);
// // 	rt.left += 50;
// // 	rt.right -= 100;
// // 	rt.top += 10;
// 	m_text3->MoveUI(&rt);
// 
// 	m_button = new CSimpleDUIButton(this, 100);
// 	rt.left = 50;
// 	rt.top = 50;
// 	rt.right = 100;
// 	rt.bottom = 70;
// 	m_button->MoveUI(&rt);
// 	m_button2 = new CSimpleDUIButton(this, 101);
// 	rt.left += 10;
// 	rt.top += 10;
// 	rt.right += 10;
// 	rt.bottom += 10;
// 	m_button2->MoveUI(&rt);

	RECT		rect;

	GetClientRect(&rect);
	m_container = new CILContainer(this);
	m_container->MoveUI(&rect);

	m_container->AddGroup();
	m_container->AddGroup();
	m_container->AddGroup();
	m_container->AddGroup();

	return TRUE;
}
/*
 *	主绘制函数
 */
LRESULT CIconListCtrl::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, 
							   LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	PAINTSTRUCT			paint;
	HDC					hDC;

	hDC = BeginPaint(&paint);

	PaintUI(m_hWnd, hDC);
// 	HDC					hMemDC;
// 	HBITMAP				hBitmap, hOldBitmap;
// 	int					nSaveDC;
// 	RECT				rect			= {0};
// 
// 	GetClientRect(&rect);
// 	
// 	// 创建内存DC
// 	hMemDC = CreateCompatibleDC(hDC);
// 	hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, 
// 		rect.bottom - rect.top);
// 	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
// 	nSaveDC = SaveDC(hMemDC);
// 	// 绘制背景
// 	HWND				hParent			= ::GetParent(m_hWnd);
// 	RECT				rtParent, rtWnd;
// 	POINT				pt, ptt;
// 
// 	::GetWindowRect(hParent, &rtParent);
// 	::GetWindowRect(m_hWnd, &rtWnd);
// 	pt.x = rtWnd.left - rtParent.left;
// 	pt.y = rtWnd.top - rtParent.top;
// 	::SetWindowOrgEx(hMemDC, pt.x, pt.y, &ptt);
// 	::SendMessage(hParent, WM_ERASEBKGND, (WPARAM)hMemDC, 0);
// 	::SetViewportOrgEx(hMemDC, pt.x, pt.y, &ptt);
// 	// 开始操作
// 	Draw(hMemDC, &rect);
// // 
// // 	::SetBkColor(hMemDC, RGB(0xff, 0x0, 0x0));
// // 	::ExtTextOut(hMemDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);	
// 	// copy
// 	BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
// 		hMemDC, 0, 0, SRCCOPY);
// 	// 恢复
// 	RestoreDC(hMemDC, nSaveDC);
// 	SelectObject(hMemDC, hOldBitmap);
// 	DeleteObject(hBitmap);
// 	DeleteObject(hMemDC);

	EndPaint(&paint);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

