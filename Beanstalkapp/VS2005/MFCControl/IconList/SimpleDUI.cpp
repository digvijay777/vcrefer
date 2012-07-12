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
	// �Ӹ�����ɾ��
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
	// ɾ���Լ����ӽڵ�
	ReleaseCapture();
	DeleteDUI(this->m_child);
}

/*
 *	ɾ���ӽڵ�
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
 *	�¼��ַ�����
 *  UI�����������Ϣ����������UI����
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
		// �ֵ����ȴ����¼�
		for(CSimpleDUIBase* node = m_brother;
			NULL != node;
			node = node->m_brother)
		{
			if(FALSE == node->m_isVisible)
			{
				continue;
			}
			// ��֤�Ƿ��ڹ�������
			node->GetUIRect(&rect);
			if( FALSE == PtInRect(&rect, pt) )
			{
				continue;	// ���ٴ���
			}

			if( node->DispatchEvent(pt, nMsg, wParam, lParam) )
			{
				return TRUE;
			}
		}
		// �¼��Ƿ��ڴ�����
		GetUIRect(&rect);
		if(FALSE == PtInRect(&rect, pt))
		{
			return FALSE;
		}
	}
	// �Ӵ������ȴ����¼�
	for(CSimpleDUIBase* node = m_child;
		NULL != node;
		node = node->m_child)
	{
		if(FALSE == node->m_isVisible)
		{
			continue;
		}
		// ��֤�Ƿ��ڹ�������
		node->GetUIRect(&rect);
		if( FALSE == PtInRect(&rect, pt) )
		{
			continue;	// ���ٴ���
		}

		if( node->DispatchEvent(pt, nMsg, wParam, lParam) )
		{
			return TRUE;
		}
	}
	// ����ע��ע���¼�������
	TranslateTrackEvent();

	// �ж��Ƿ��ڵ�ǰ�㴦���¼�
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
 *	�ƶ�����
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
 *	�ػ�
 */
void CSimpleDUIBase::Draw(HDC hDC, LPRECT lpRect)
{
	// �����Լ�
	if( m_isVisible )
	{
		OnDraw(hDC, lpRect);
	}
	// �����ֵܴ���
	if(NULL != m_brother)
	{
		m_brother->Draw(hDC, lpRect);
	}
	// �����Ӵ���
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
 *	��ʾ����
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
 *	�¼�
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
 *	����ת��
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
 *	��ȡ����
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
 *	�ػ洰��
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
 *	��ȡpanel
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
 *	�¼�����
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
 *	�¼�ת��
 */
BOOL CSimpleDUIRoot::TranslateEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	m_panelRoot.hWnd = hWnd;
	// ����¼�
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
		// �¼����ȴ���
		if(NULL != m_panelRoot.captureUI)
		{
			return m_panelRoot.captureUI->DispatchEvent(pt, nMsg, wParam, lParam);
		}
		// �Ӵ���ӹ��¼�
		if(NULL != GetChild())
		{
			bRet = GetChild()->DispatchEvent(pt, nMsg, wParam, lParam);
			if(FALSE != bRet)
			{
				return TRUE;
			}
		}
		// ������ӹ��¼�
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
 *	���ƺ���
 */
void CSimpleDUIRoot::Paint(HWND hWnd, HDC hDC)
{
	HDC					hMemDC;
	HBITMAP				hBitmap, hOldBitmap;
	int					nSaveDC;
	RECT				rect			= {0};
	HFONT				hFont, hOldFont;

	::GetClientRect(hWnd, &rect);
	// �����ڴ�DC
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, 
		rect.bottom - rect.top);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ���Ʊ���
	nSaveDC = SaveDC(hMemDC);
	EraseBkgnd(hMemDC, hWnd);
	::RestoreDC(hMemDC, nSaveDC);
	// ��ʼ����
	nSaveDC = SaveDC(hMemDC);
	hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);
	SetBkMode(hMemDC, TRANSPARENT);

	Draw(hMemDC, &rect);

	SelectObject(hMemDC, hOldFont);
	::RestoreDC(hMemDC, nSaveDC);
	// ����
	::BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hMemDC, 0, 0, SRCCOPY);
	// �ָ�
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
 *	��ť�¼�
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
 *	���ư�ť
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