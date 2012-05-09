// TimeSelectCtrl3.cpp : implementation file
//

#include "stdafx.h"
#include "TimeSelectCtrl3.h"


// CTimeSelectCtrl3
CTimeSelectCtrl3::CTimeSelectCtrl3()
{
	m_nSplit = -1;
	m_pData = NULL;
	m_pText = NULL;
	m_nButtonDown = -1;
}

CTimeSelectCtrl3::~CTimeSelectCtrl3()
{
}

/*
 *	绘制函数
 */
LRESULT CTimeSelectCtrl3::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	PAINTSTRUCT			paint			= {0};
	HDC					hDC				= NULL;
	RECT				rect			= {0};
	HDC					hMemDC			= NULL;
	HBITMAP				hBitmap			= NULL;
	int					nSaveDC			= 0;

	bHandled = TRUE;
	hDC = BeginPaint(&paint);
	GetClientRect(&rect);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, 
		rect.bottom - rect.top); // 高度加12， 以画非显示图形
	nSaveDC = SaveDC(hMemDC);
	// 选择设备
	::SelectObject(hMemDC, hBitmap);
	// 绘制背景
	Draw_Bk(m_hWnd, hMemDC);
	::SelectObject(hMemDC, GetFont());
	// 绘制区域
	DrawItem(hMemDC, &rect);
	// 复制内存DC
	::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
		, hMemDC, 0, 0, SRCCOPY);

	RestoreDC(hDC, nSaveDC);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	EndPaint(&paint);
	return 0;
}

/*
 *	绘制项
 */
void CTimeSelectCtrl3::DrawItem(HDC hDC, LPRECT lpRect)
{
	if(0 >= m_nSplit || NULL == m_pData)
	{
		return;
	}

	int			nGrid			= (lpRect->right - lpRect->left - 1) / m_nSplit;
	int			nWidth			= nGrid * m_nSplit;
	RECT		rt				= *lpRect;
	WCHAR		szText[1024]	= {0};
	LPCWSTR		pText			= NULL;

	rt.right = nWidth + 1;
	// 绘制边框
	::SetBkColor(hDC, RGB(0x42, 0xA8, 0x38));
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
	rt.top++;
	rt.bottom--;
	rt.left++;
	if(NULL != m_pText)
	{
		wcscpy(szText, m_pText);
		pText = wcstok(szText, L";");
	}
	// 开始绘制
	for(int i = 0; i < m_nSplit; i++)
	{
		RECT	rtItem	= rt;

		rtItem.left += i * nGrid;
		rtItem.right = rtItem.left + nGrid - 1;
		if(m_pData[i])
		{
			// 选择区绘制
			RECT		rtGrid		= rtItem;

			::SetBkColor(hDC, RGB(0x8A, 0xE1, 0x8D));
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rtGrid, NULL, 0, NULL);
			rtGrid.top++;
			rtGrid.left++;
			::SetBkColor(hDC, RGB(0x58, 0xD3, 0x59));
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rtGrid, NULL, 0, NULL);			
		}
		else
		{
			// 非选择区绘制
			RECT		rtGrid		= rtItem;

			::SetBkColor(hDC, RGB(0xF7, 0xFB, 0xF5));
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rtGrid, NULL, 0, NULL);
			rtGrid.top++;
			rtGrid.left++;
			::SetBkColor(hDC, RGB(0xEC, 0xEB, 0xEB));
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rtGrid, NULL, 0, NULL);			
		}
		// 绘制文本
		if(NULL == pText)
		{
			continue;
		}
		::SetTextColor(hDC, (m_pData[i])?RGB(0xff, 0xff, 0xff):RGB(0x0, 0x0, 0x0));
		::DrawTextW(hDC, pText, wcslen(pText), &rtItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pText = wcstok(NULL, L";");
	}
}

void CTimeSelectCtrl3::Draw_Bk(HWND hWnd, HDC hDC)
{
	HWND 		hParent		= NULL;
	RECT		WndRect		= {0};
	RECT		ParRect		= {0};
	POINT		OffPt		= {0};
	POINT		OriPt		= {0};

	hParent = ::GetParent(hWnd);
	::GetWindowRect(hWnd, &WndRect);
	::GetWindowRect(hParent, &ParRect);

	OffPt.x = WndRect.left - ParRect.left;
	OffPt.y = WndRect.top - ParRect.top;

	::SetWindowOrgEx(hDC, OffPt.x, OffPt.y, &OriPt);
	::SendMessage(hParent, WM_ERASEBKGND, (WPARAM)hDC, 0);
	::SetViewportOrgEx(hDC, OffPt.x, OffPt.y, &OriPt);
}

/*
 *	设置数据
 */
void CTimeSelectCtrl3::SetData(bool* pData, int nCount, WCHAR* pText /* = NULL */)
{
	m_nSplit = nCount;
	m_pData = pData;
	m_pText = pText;
}

int CTimeSelectCtrl3::HitTestItem(POINT pt)
{
	int			nItem	= -1;
	RECT		rt;
	int			nGrid	= 0;

	if(0 >= m_nSplit)
	{
		return nItem;
	}

	GetClientRect(&rt);
	nGrid = (rt.right - rt.left) / m_nSplit;
	rt.right = rt.left + nGrid * m_nSplit;
	rt.left++;
	rt.top++;
	rt.bottom--;
	if(!::PtInRect(&rt, pt))
	{
		return nItem;
	}

	nItem = (pt.x - rt.left) / nGrid;
	return nItem;
}

LRESULT CTimeSelectCtrl3::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT		pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	int			nItem	= -1;
	
	nItem = HitTestItem(pt);
	if(nItem >= 0 && nItem < m_nSplit)
	{
		if(m_nButtonDown == nItem)
		{
			m_pData[nItem] = !m_pData[nItem];
			Invalidate();
		}
	}

	m_nButtonDown = -1;
	return 0;
}

LRESULT CTimeSelectCtrl3::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT		pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	m_nButtonDown = HitTestItem(pt);
	return 0;
}
