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
CILContainer::CILContainer()
{
	m_nCurrentGroup = -1;
	m_nSwitchGroup = 0;
	m_navigatebar = new CSimpleDUIPanel(this, RGB(0xff, 0xff, 0));
}

CILContainer::~CILContainer()
{

}

void CILContainer::OnUIDraw(HDC hDC, LPRECT lpRect)
{

}

BOOL CILContainer::TranslateUIEvent(HWND hWnd, UINT nMsg,
									WPARAM wParam, LPARAM lParam)
{
// 	if(WM_TIMER == nMsg && wParam == (WPARAM)this)
// 	{
// 		if(m_nSwitchGroup != m_nCurrentGroup
// 			&& m_nCurrentGroup < m_groups.size()
// 			&& m_nSwitchGroup < m_groups.size())
// 		{
// 			RECT		rect;
// 			RECT		rtCur, rtSwt;
// 
// 			m_groups[m_nCurrentGroup]->GetUIRect(&rect);
// 			if(m_nSwitchGroup > m_nCurrentGroup)
// 			{
// 				rtCur = rect;
// 				rtCur.left = rect.left - 10;
// 				rtCur.right = rect.right - 10;
// 
// 				rtSwt = rect;
// 				rtSwt.left = rect.right - 10;
// 				rtSwt.right = rect.right + rect.right - rect.left - 10;
// 
// 				if(rtCur.right < 0)
// 				{
// 					KillTimer(hWnd, wParam);
// 					ShowGroup(m_nSwitchGroup);
// 					return TRUE;
// 				}
// 			}
// 			else
// 			{
// 				rtCur = rect;
// 				rtCur.left = rect.left + 10;
// 				rtCur.right = rect.right + 10;
// 
// 				rtSwt = rect;
// 				rtSwt.left = rect.right + 10;
// 				rtSwt.right = rect.right + rect.right - rect.left + 10;	
// 
// 				if(rtCur.left > (rect.right - rect.left))
// 				{
// 					KillTimer(hWnd, wParam);
// 					ShowGroup(m_nSwitchGroup);
// 					return TRUE;
// 				}
// 			}
// 
// 			m_groups[m_nCurrentGroup]->MoveUI(&rtCur, FALSE);
// 			m_groups[m_nSwitchGroup]->ShowUI(TRUE);
// 			m_groups[m_nSwitchGroup]->MoveUI(&rtSwt, TRUE);
// 		}
// 		else
// 		{
// 			KillTimer(hWnd, wParam);
// 		}
// 
// 		return TRUE;
// 	}

	return CSimpleDUIRoot::TranslateUIEvent(hWnd, nMsg, wParam, lParam);
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
	m_navigates.push_back(new CSimpleDUIButton(m_navigatebar, 400 + m_groups.size() - 1));

	return TRUE;
}

BOOL CILContainer::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;

	if(WM_LBUTTONDOWN == nMsg)
	{
		if(m_groups.size() == 0)
		{
			return FALSE;
		}

		m_nSwitchGroup++;
		m_nSwitchGroup = m_nSwitchGroup % m_groups.size();
		ShowGroup(m_nSwitchGroup);
		return TRUE;
	}
	else if(WM_RBUTTONDOWN == nMsg)
	{
		if(m_groups.size() == 0)
		{
			return FALSE;
		}

		m_nSwitchGroup += m_groups.size() - 1;
		m_nSwitchGroup = m_nSwitchGroup % m_groups.size();
		ShowGroup(m_nSwitchGroup);
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
/*
 *	调整导航栏位置
 */
BOOL CILContainer::UpdateNaviageBar()
{
	RECT		rtBar, rtNv;
	int			nSize		= 40;
	int			nSpace		= 10;
	int			nLeft		= 0;
	int			nTop		= 0;

	m_navigatebar->GetUIRect(&rtBar);
	nLeft = ((rtBar.right - rtBar.left) - (nSize + nSpace) * m_navigates.size()
		- nSpace) / 2;
	nTop = (rtBar.bottom - rtBar.top - nSize) / 2;
	for(size_t i = 0; i < m_navigates.size(); i++)
	{
		rtNv.left = nLeft;
		rtNv.top = nTop;
		rtNv.right = rtNv.left + nSize;
		rtNv.bottom = rtNv.top + nSize;
		nLeft += nSize + nSpace;

		m_navigates[i]->MoveUI(&rtNv, FALSE);
	}

	m_navigatebar->ForegroundUI();

	return TRUE;
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

	RECT	rect;

	GetClientRect(&rect);
	MoveUI(&rect);
	rect.top = rect.bottom - 60;
	rect.bottom -= 10;
	m_navigatebar->MoveUI(&rect);

	AddGroup();
	AddGroup();
	AddGroup();
	AddGroup();

	ShowGroup(0);
	UpdateNaviageBar();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

