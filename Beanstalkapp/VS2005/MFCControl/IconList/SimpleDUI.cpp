#include "StdAfx.h"
#include "SimpleDUI.h"
#include <assert.h>

CSimpleDUIBase::CSimpleDUIBase(CSimpleDUIBase* parent)
: m_parent(parent)
{
	m_child = NULL;
	m_brother = NULL;
	m_isVisible = TRUE;
	memset(&m_rect, 0, sizeof(m_rect));
	
	if(NULL != parent)
	{
		CSimpleDUIBase*		node		= parent;

		if(NULL == node->m_child)
		{
			node->m_child = this;
		}
		else
		{
			node = node->m_child;
			while(node->m_brother)
			{
				node = node->m_brother;
			}

			node->m_brother = this;
		}
	}
}

CSimpleDUIBase::~CSimpleDUIBase()
{
	// 从父树中删除
	if(NULL != m_parent)
	{
		CSimpleDUIBase*		node		= m_parent;

		if(this == node->m_child)
		{
			node->m_child = m_brother;
		}
		else
		{
			node = node->m_child;
			for(; node; node = node->m_brother)
			{
				if(this == node->m_brother)
				{
					node->m_brother = this->m_brother;
					break;
				}
			}
		}
	}
	// 删除自己和子节点
	ReleaseUICapture();
	DeleteDUI(this->m_child);
}

/*
 *	删除子节点
 */
void CSimpleDUIBase::DeleteDUI(CSimpleDUIBase* pUI)
{
	if(NULL == pUI)
	{
		return ;
	}

	DeleteDUI(pUI->m_brother);
	DeleteDUI(pUI->m_child);
	delete pUI;
}

/*
 *	事件分发处理
 *  UI处理了这个消息不再由其它UI处理
 */
BOOL CSimpleDUIBase::DispatchUIEvent(POINT pt, UINT nMsg,
								   WPARAM wParam, LPARAM lParam)
{
	BOOL		bRet;
	RECT		rect;

	if(FALSE == m_isVisible)
	{
		return FALSE;
	}
	// 处理接管事件
	if(NULL != GetUIRoot() && this == GetUIRoot()->captureUI)
	{
		return OnUIEvent(nMsg, wParam, lParam);
	}
	// 子窗体处理事件
	for(CSimpleDUIBase* node = m_child;
		NULL != node;
		node = node->m_brother)
	{
		if(FALSE == node->m_isVisible)
		{
			continue;
		}
		// 验证是否在关心区域
		node->GetUIRect(&rect);
		if( FALSE == PtInRect(&rect, pt) )
		{
			continue;	// 加速处理
		}

		if( node->DispatchUIEvent(pt, nMsg, wParam, lParam) )
		{
			return TRUE;
		}
	}
	// 处理注册注销事件处理函数
	TranslateTrackEvent();

	// 判断是否在当前层处理事件
	bRet = OnUIEvent(nMsg, wParam, lParam);

	return bRet;
}

inline void CSimpleDUIBase::TranslateTrackEvent()
{
	if(NULL != GetUIRoot()
		&& NULL != GetUIRoot()->trackUI
		&& this != GetUIRoot()->trackUI)
	{
		GetUIRoot()->trackUI->OnUIEvent(GetUIRoot()->trackMsg, 0, 0);
		GetUIRoot()->trackUI = NULL;
	}
}
/*
 *	移动窗体
 */
void CSimpleDUIBase::MoveUI(LPRECT lpRect, BOOL bInvalidate /* = TRUE */)
{
	m_rect = *lpRect;

	if( bInvalidate )
	{
		UIInvalidate();
	}
}

/*
 *	重绘
 */
void CSimpleDUIBase::DrawUI(HDC hDC, LPRECT lpRect)
{
	// 绘制自己
	if( m_isVisible )
	{
		OnUIDraw(hDC, lpRect);
	}
	// 绘制兄弟窗体
	if(NULL != m_brother)
	{
		m_brother->DrawUI(hDC, lpRect);
	}
	// 绘制子窗体
	if(FALSE == m_isVisible)
	{
		return;
	}
	if(NULL != m_child)
	{
		m_child->DrawUI(hDC, lpRect);
	}
}

BOOL CSimpleDUIBase::IsVisible()
{
	return m_isVisible;
}
/*
 *	显示窗体
 */
void CSimpleDUIBase::ShowUI(BOOL bShow)
{
	RECT		rect;

	if(m_isVisible != bShow)
	{
		m_isVisible = bShow;
		GetUIRect(&rect);
		UIInvalidate(&rect);
	}
}
/*
 *	事件
 */
void CSimpleDUIBase::SetUICapture()
{
	if(NULL != GetUIRoot() && NULL == GetUIRoot()->captureUI)
	{
		::SetCapture(GetUIRoot()->hWnd);
		GetUIRoot()->captureUI = this;
	}
}
void CSimpleDUIBase::ReleaseUICapture()
{
	if(NULL != GetUIRoot() && NULL != GetUIRoot()->captureUI)
	{
		::ReleaseCapture();
		GetUIRoot()->captureUI = NULL;
	}
}

void CSimpleDUIBase::SetUIFocus()
{
	if(NULL != GetUIRoot())
	{
		GetUIRoot()->focusUI = this;
	}
}

void CSimpleDUIBase::KillUIFocus()
{
	if(NULL != GetUIRoot() && this == GetUIRoot()->focusUI)
	{
		GetUIRoot()->focusUI = NULL;
	}
}

void CSimpleDUIBase::TrackEvent(UINT nMsg)
{
	if(NULL == GetUIRoot())
	{
		return;
	}

	if(0 == nMsg)
	{
		GetUIRoot()->trackUI = NULL;
		GetUIRoot()->trackMsg = 0;
	}
	else
	{
		GetUIRoot()->trackUI = this;
		GetUIRoot()->trackMsg = nMsg;
	}
}
/*
 *	区域转换
 */
void CSimpleDUIBase::RectToPanel(RECT* rect)
{
	rect->left += m_rect.left;
	rect->top += m_rect.top;
	rect->right += m_rect.left;
	rect->bottom += m_rect.top;

	if(NULL != m_parent)
	{
		m_parent->RectToPanel(rect);
	}
}
/*
 *	获取区域
 */
void CSimpleDUIBase::GetUIRect(RECT* rect)
{
	*rect = m_rect;

	if(NULL != m_parent)
	{
		m_parent->RectToPanel(rect);
	}
}
/*
 *	置前UI
 */
void CSimpleDUIBase::ForegroundUI()
{
	CSimpleDUIBase*		parent		= m_parent;
	CSimpleDUIBase*		node;

	if(NULL == parent || NULL == m_brother)
	{
		return;
	}

	if(this == parent->m_child)
	{
		parent->m_child = this->m_brother;
		node = this->m_brother;
		this->m_brother = NULL;
	}
	else
	{
		node = parent->m_child;
		while(this != node->m_brother)
		{
			node = node->m_brother;
		}
		
		node->m_brother = this->m_brother;
		node = this->m_brother;
		this->m_brother = NULL;
	}

	while(NULL != node->m_brother)
	{
		node = node->m_brother;
	}

	node->m_brother = this;
	UIInvalidate(NULL);
}
/*
 *	重绘窗体
 */
void CSimpleDUIBase::UIInvalidate(LPRECT lpRect /* = NULL */)
{
	if(NULL != m_parent)
	{
		m_parent->UIInvalidate(lpRect);
		return;
	}

	assert(false);
}
/*
 *	获取panel
 */
PDUI_ROOT CSimpleDUIBase::GetUIRoot()
{
	if(NULL != m_parent)
	{
		return m_parent->GetUIRoot();
	}

	return NULL;
}
/*
 *	事件处理
 */
BOOL CSimpleDUIBase::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

void CSimpleDUIBase::MergerRect(RECT* des, LPRECT rt1, LPRECT rt2)
{
	des->left = max(rt1->left, rt2->left);
	des->right = min(rt1->right, rt2->right);
	des->top = max(rt1->top, rt2->top);
	des->bottom = min(rt1->bottom, rt2->bottom);
}
//////////////////////////////////////////////////////////////////////////
CSimpleDUIRoot::CSimpleDUIRoot()
: CSimpleDUIBase(NULL)
{
	memset(&m_root, 0, sizeof(m_root));
	m_bTrackEvent = FALSE;
}

CSimpleDUIRoot::~CSimpleDUIRoot()
{

}
/*
 *	事件转换
 */
BOOL CSimpleDUIRoot::TranslateUIEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	m_root.hWnd = hWnd;
	// 鼠标事件
	if(WM_LBUTTONDOWN == nMsg || WM_LBUTTONUP == nMsg 
		|| WM_LBUTTONDBLCLK == nMsg || WM_RBUTTONDOWN == nMsg
		|| WM_RBUTTONUP == nMsg || WM_RBUTTONDBLCLK == nMsg
		|| WM_MOUSEMOVE == nMsg || WM_MOUSEHOVER == nMsg
		|| WM_MOUSELEAVE == nMsg)
	{
		POINT			pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		BOOL			bRet;

		if(WM_MOUSEMOVE == nMsg)
		{
			if(FALSE == m_bTrackEvent)
			{
				TRACKMOUSEEVENT			tme		= {0};

				tme.cbSize = sizeof(tme);
				tme.hwndTrack = hWnd;
				tme.dwFlags = TME_LEAVE | TME_HOVER;
				tme.dwHoverTime = HOVER_DEFAULT;
				m_bTrackEvent = TrackMouseEvent(&tme);
			}
		}
		if(WM_MOUSELEAVE == nMsg)
		{
			m_bTrackEvent = FALSE;
		}
		// 事件优先处理
		if(NULL != m_root.captureUI)
		{
			return m_root.captureUI->DispatchUIEvent(pt, nMsg, wParam, lParam);
		}
		// 子窗体接管事件
		return DispatchUIEvent(pt, nMsg, wParam, lParam);
	}
	else if(WM_PAINT == nMsg)
	{
		HDC					hDC;
		PAINTSTRUCT			ps			= {0};

		hDC = BeginPaint(hWnd, &ps);
		PaintUI(hWnd, hDC);
		EndPaint(hWnd, &ps);
	}

	return FALSE;
}
/*
 *	绘制函数
 */
void CSimpleDUIRoot::PaintUI(HWND hWnd, HDC hDC)
{
	HDC					hMemDC;
	HBITMAP				hBitmap, hOldBitmap;
	int					nSaveDC;
	RECT				rect			= {0};
	HFONT				hFont, hOldFont;

	::GetClientRect(hWnd, &rect);
	// 创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, 
		rect.bottom - rect.top);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 绘制背景
	nSaveDC = SaveDC(hMemDC);
	EraseUIBkgnd(hMemDC, hWnd);
	::RestoreDC(hMemDC, nSaveDC);
	// 开始操作
	nSaveDC = SaveDC(hMemDC);
	hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);
	SetBkMode(hMemDC, TRANSPARENT);

	DrawUI(hMemDC, &rect);

	SelectObject(hMemDC, hOldFont);
	::RestoreDC(hMemDC, nSaveDC);
	// 复制
	::BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hMemDC, 0, 0, SRCCOPY);
	// 恢复
	::SelectObject(hMemDC, hOldBitmap);
	::DeleteObject(hBitmap);
	::DeleteObject(hMemDC);
}

PDUI_ROOT CSimpleDUIRoot::GetUIRoot()
{
	return &m_root;
}

void CSimpleDUIRoot::EraseUIBkgnd(HDC hDC, HWND hWnd)
{
	HWND				hParent			= ::GetParent(hWnd);
	RECT				rtParent, rtWnd;
	POINT				pt, ptt;

	::GetWindowRect(hParent, &rtParent);
	::GetWindowRect(hWnd, &rtWnd);
	pt.x = rtWnd.left - rtParent.left;
	pt.y = rtWnd.top - rtParent.top;
	::SetWindowOrgEx(hDC, pt.x, pt.y, &ptt);
	::SendMessage(hParent, WM_ERASEBKGND, (WPARAM)hDC, 0);
	::SetViewportOrgEx(hDC, pt.x, pt.y, &ptt);
}

void CSimpleDUIRoot::UIInvalidate(LPRECT lpRect)
{
	if(NULL == m_root.hWnd)
	{
		return;
	}

	::InvalidateRect(m_root.hWnd, lpRect, FALSE);
}

void CSimpleDUIRoot::OnUIDraw(HDC hDC, LPRECT lpRect)
{

}



//////////////////////////////////////////////////////////////////////////
CSimpleDUIText::CSimpleDUIText(LPCWSTR lpText, CSimpleDUIBase* parent)
: CSimpleDUIBase(parent)
{
	m_text = lpText;
}

CSimpleDUIText::~CSimpleDUIText()
{

}

void CSimpleDUIText::OnUIDraw(HDC hDC, LPRECT lpRect)
{
	RECT		rect;

	GetUIRect(&rect);
	MergerRect(&rect, &rect, lpRect);
	DrawText(hDC, m_text.c_str(), (int)m_text.size(), &rect, DT_SINGLELINE | DT_CENTER);
}

void CSimpleDUIText::SetText(LPCWSTR lpText)
{
	m_text = lpText;
}

//////////////////////////////////////////////////////////////////////////
CSimpleDUIButton::CSimpleDUIButton(CSimpleDUIBase* parent, UINT uID)
: CSimpleDUIBase(parent)
, m_uID(uID)
{
	m_status = 0;
}

CSimpleDUIButton::~CSimpleDUIButton()
{

}
/*
 *	按钮事件
 */
BOOL CSimpleDUIButton::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
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
		m_status = 2;
		TrackEvent(0);
		SetUICapture();
		UIInvalidate(&rect);
	}
	else if(WM_LBUTTONUP == nMsg)
	{
		POINT		pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

		ReleaseUICapture();
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
/*
 *	绘制按钮
 */
void CSimpleDUIButton::OnUIDraw(HDC hDC, LPRECT lpRect)
{
	RECT		rect;

	GetUIRect(&rect);
	MergerRect(&rect, &rect, lpRect);

	if(1 == m_status)
	{
		::SetBkColor(hDC, RGB(0xff, 0x0, 0x0));
	}
	else if(2 == m_status)
	{
		::SetBkColor(hDC, RGB(0x0, 0xff, 0x0));
	}
	else
	{
		::SetBkColor(hDC, RGB(0x0, 0x0, 0xff));
	}

	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

//////////////////////////////////////////////////////////////////////////
CSimpleDUIPanel::CSimpleDUIPanel(CSimpleDUIBase* parent, COLORREF col)
: CSimpleDUIBase(parent)
, m_color(col)
{

}

CSimpleDUIPanel::~CSimpleDUIPanel()
{

}

void CSimpleDUIPanel::OnUIDraw(HDC hDC, LPRECT lpRect)
{
	RECT		rect;

	GetUIRect(&rect);
	MergerRect(&rect, &rect, lpRect);

	::SetBkColor(hDC, m_color);
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}