#include "stdafx.h"
#include "XCaptionControl.h"

#define STRING_WNDPROCPROP			"__XCaptionCtrl__"

std::vector<CXCaptionCtrl*>		CXCaptionCtrl::s_vctXCaptionCtrl;
WNDPROC							CXCaptionCtrl::s_OldWndProc				= NULL;

CXCaptionCtrl::CXCaptionCtrl()
{

}

CXCaptionCtrl::~CXCaptionCtrl()
{
	std::vector<CXCaptionCtrl*>::iterator		item;

	for(item = s_vctXCaptionCtrl.begin(); 
		item != s_vctXCaptionCtrl.end(); 
		item++)
	{
		if(this == (*item))
		{
			s_vctXCaptionCtrl.erase(item);
			break;
		}
	}
}

/*
 *	创建控件
 */
BOOL CXCaptionCtrl::CreateCtrl(CString& text, CRect& rect, UINT ID)
{
	// 负值
	m_uID = ID;
	m_rect = rect;
	m_strText = text;
	m_pMainWnd = AfxGetMainWnd();
	
	ASSERT(NULL != m_pMainWnd);
	// 设置消息
	if(NULL == s_OldWndProc)
	{
		s_OldWndProc = (WNDPROC)GetWindowLong(m_pMainWnd->m_hWnd, GWL_WNDPROC);
		if(NULL == s_OldWndProc)
			return FALSE;
		SetWindowLong(m_pMainWnd->m_hWnd, GWL_WNDPROC, (LONG)XCaptionCtrlCallWndProc);
		s_vctXCaptionCtrl.push_back(this);
	}
	return FALSE;
}

/*
 *	绘制函数
 */
void CXCaptionCtrl::OnDraw(CDC *pDC)
{

}

/*
 *	窗体回调函数
 */
HRESULT CXCaptionCtrl::XCaptionCtrlCallWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(NULL != s_OldWndProc);
	
	LRESULT			lRet		= 0;

	
	switch(uMsg)
	{
	case WM_NCPAINT:
	case WM_NCACTIVATE:
		{
			CWnd*			pMain		= AfxGetMainWnd();
			CDC*			pWndDC		= pMain->GetWindowDC();
			CRect			rtCaption;
			CRect			rtClient;
			
			pMain->GetWindowRect(&rtCaption);
			pMain->GetClientRect(&rtClient);
			rtCaption.bottom = rtClient.top;
			rtCaption.MoveToXY(0, 0);

			lRet = s_OldWndProc(hWnd, uMsg, wParam, lParam);
			ReleaseDC(pMain->m_hWnd, pWndDC->m_hDC);
		}	
		break;
	default:
		lRet = s_OldWndProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}
