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
	ReleaseCapture();
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

CSimpleDUIBase* CSimpleDUIBase::GetChild()
{
	return m_child;
}
/*
 *	事件分发处理
 *  UI处理了这个消息不再由其它UI处理
 */
BOOL CSimpleDUIBase::DispatchEvent(POINT pt, UINT nMsg,
								   WPARAM wParam, LPARAM lParam)
{
	BOOL		bRet;
	RECT		rect;

	if(FALSE == m_isVisible)
	{
		return FALSE;
	}

	if(NULL != GetPaneRoot() && this != GetPaneRoot()->captureUI)
	{
		// 兄弟优先处理事件
		for(CSimpleDUIBase* node = m_brother;
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

			if( node->DispatchEvent(pt, nMsg, wParam, lParam) )
			{
				return TRUE;
			}
		}
		// 事件是否在窗体内
		GetUIRect(&rect);
		if(FALSE == PtInRect(&rect, pt))
		{
			return FALSE;
		}
	}
	// 子窗体优先处理事件
	for(CSimpleDUIBase* node = m_child;
		NULL != node;
		node = node->m_child)
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

		if( node->DispatchEvent(pt, nMsg, wParam, lParam) )
		{
			return TRUE;
		}
	}
	// 处理注册注销事件处理函数
	TranslateTrackEvent();

	// 判断是否在当前层处理事件
	bRet = OnEvent(nMsg, wParam, lParam);

	return bRet;
}

inline void CSimpleDUIBase::TranslateTrackEvent()
{
	if(NULL != GetPaneRoot()
		&& NULL != GetPaneRoot()->trackUI
		&& this != GetPaneRoot()->trackUI)
	{
		GetPaneRoot()->trackUI->OnEvent(GetPaneRoot()->trackMsg, 0, 0);
		GetPaneRoot()->trackUI = NULL;
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
		Invalidate();
	}
}

/*
 *	重绘
 */
void CSimpleDUIBase::Draw(HDC hDC, LPRECT lpRect)
{
	// 绘制自己
	if( m_isVisible )
	{
		OnDraw(hDC, lpRect);
	}
	// 绘制兄弟窗体
	if(NULL != m_brother)
	{
		m_brother->Draw(hDC, lpRect);
	}
	// 绘制子窗体
	if(FALSE == m_isVisible)
	{
		return;
	}
	if(NULL != m_child)
	{
		m_child->Draw(hDC, lpRect);
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
		Invalidate(&rect);
	}
}
/*
 *	事件
 */
void CSimpleDUIBase::SetCapture()
{
	if(NULL != GetPaneRoot() && NULL == GetPaneRoot()->captureUI)
	{
		::SetCapture(GetPaneRoot()->hWnd);
		GetPaneRoot()->captureUI = this;
	}
}
void CSimpleDUIBase::ReleaseCapture()
{
	if(NULL != GetPaneRoot() && NULL != GetPaneRoot()->captureUI)
	{
		::ReleaseCapture();
		GetPaneRoot()->captureUI = NULL;
	}
}

void CSimpleDUIBase::SetFocus()
{
	if(NULL != GetPaneRoot())
	{
		GetPaneRoot()->focusUI = this;
	}
}

void CSimpleDUIBase::KillFocus()
{
	if(NULL != GetPaneRoot() && this == GetPaneRoot()->focusUI)
	{
		GetPaneRoot()->focusUI = NULL;
	}
}

void CSimpleDUIBase::TrackEvent(UINT nMsg)
{
	if(NULL == GetPaneRoot())
	{
		return;
	}

	if(0 == nMsg)
	{
		GetPaneRoot()->trackUI = NULL;
		GetPaneRoot()->trackMsg = 0;
	}
	else
	{
		GetPaneRoot()->trackUI = this;
		GetPaneRoot()->trackMsg = nMsg;
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
 *	重绘窗体
 */
void CSimpleDUIBase::Invalidate(LPRECT lpRect /* = NULL */)
{
	if(NULL != m_parent)
	{
		m_parent->Invalidate(lpRect);
		return;
	}

	assert(false);
}
/*
 *	获取panel
 */
PDUI_ROOT CSimpleDUIBase::GetPaneRoot()
{
	if(NULL != m_parent)
	{
		return m_parent->GetPaneRoot();
	}

	return NULL;
}
/*
 *	事件处理
 */
BOOL CSimpleDUIBase::OnEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
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
	memset(&m_panelRoot, 0, sizeof(m_panelRoot));
	m_bTrackEvent = FALSE;
}

CSimpleDUIRoot::~CSimpleDUIRoot()
{

}
/*
 *	事件转换
 */
BOOL CSimpleDUIRoot::TranslateEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	m_panelRoot.hWnd = hWnd;
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
		if(NULL != m_panelRoot.captureUI)
		{
			return m_panelRoot.captureUI->DispatchEvent(pt, nMsg, wParam, lParam);
		}
		// 子窗体接管事件
		if(NULL != GetChild())
		{
			bRet = GetChild()->DispatchEvent(pt, nMsg, wParam, lParam);
			if(FALSE != bRet)
			{
				return TRUE;
			}
		}
		// 本窗体接管事件
		TranslateTrackEvent();
	}
	else if(WM_PAINT == nMsg)
	{
		HDC					hDC;
		PAINTSTRUCT			ps			= {0};

		hDC = BeginPaint(hWnd, &ps);
		Paint(hWnd, hDC);
		EndPaint(hWnd, &ps);
	}

	return FALSE;
}
/*
 *	绘制函数
 */
void CSimpleDUIRoot::Paint(HWND hWnd, HDC hDC)
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
	EraseBkgnd(hMemDC, hWnd);
	::RestoreDC(hMemDC, nSaveDC);
	// 开始操作
	nSaveDC = SaveDC(hMemDC);
	hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);
	SetBkMode(hMemDC, TRANSPARENT);

	Draw(hMemDC, &rect);

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

PDUI_ROOT CSimpleDUIRoot::GetPaneRoot()
{
	return &m_panelRoot;
}

void CSimpleDUIRoot::EraseBkgnd(HDC hDC, HWND hWnd)
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

void CSimpleDUIRoot::Invalidate(LPRECT lpRect)
{
	if(NULL == m_panelRoot.hWnd)
	{
		return;
	}

	::InvalidateRect(m_panelRoot.hWnd, lpRect, FALSE);
}

void CSimpleDUIRoot::OnDraw(HDC hDC, LPRECT lpRect)
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

void CSimpleDUIText::OnDraw(HDC hDC, LPRECT lpRect)
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
BOOL CSimpleDUIButton::OnEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	RECT		rect;

	GetUIRect(&rect);
	if(WM_MOUSEMOVE == nMsg)
	{
		if(0 == m_status)
		{
			m_status = 1;
			Invalidate(&rect);
			TrackEvent(WM_MOUSELEAVE);
		}
	}
	else if(WM_MOUSELEAVE == nMsg)
	{
		m_status = 0;
		Invalidate(&rect);
	}
	else if(WM_LBUTTONDOWN == nMsg)
	{
		m_status = 2;
		TrackEvent(0);
		SetCapture();
		Invalidate(&rect);
	}
	else if(WM_LBUTTONUP == nMsg)
	{
		POINT		pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

		ReleaseCapture();
		if(PtInRect(&rect, pt))
		{
			m_status = 1;
			TrackEvent(WM_MOUSELEAVE);
			SendMessage(GetPaneRoot()->hWnd, WM_COMMAND, m_uID, 0);
		}
		else
		{
			m_status = 0;
		}
		Invalidate(&rect);
	}

	return TRUE;
}
/*
 *	绘制按钮
 */
void CSimpleDUIButton::OnDraw(HDC hDC, LPRECT lpRect)
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