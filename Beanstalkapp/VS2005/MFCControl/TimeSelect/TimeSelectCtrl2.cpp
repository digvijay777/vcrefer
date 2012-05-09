#include "StdAfx.h"
#include "TimeSelectCtrl2.h"

#define DR_BODY				-1
#define DR_NORTHWEST		0
#define DR_NORTH			1
#define DR_WEST				2
#define DR_CENTER			3

#define COLOR_FRAME			RGB(0x3F, 0xA8, 0x41)

// 左边星期背景色
COLORREF CTimeSelectCtrl2::s_WeekBk[7] = {
	RGB(0xDD, 0xF5, 0xA6), RGB(0xEB, 0xC2, 0xAF), RGB(0xC0, 0xCA, 0xF7)
	, RGB(0xA8, 0xE9, 0xCF), RGB(0xD8, 0xC1, 0xEE), RGB(0x7E, 0xD3, 0xD5)
	, RGB(0xEE, 0x9B, 0xA2)
};
// 顶部背景色低高索引
COLORREF CTimeSelectCtrl2::s_TopBk[2] = {
	RGB(0xD7, 0xEE, 0xFC), RGB(0x97, 0xD4, 0xF3)
};
// 灰度算法 [R,G,B] = (r * 3 + g * 6 + g) / 10;

CTimeSelectCtrl2::CTimeSelectCtrl2(void)
{
	m_penLine = NULL;
	wcscpy(m_szWeek, L"日一二三四五六");
	memset(m_WeekFlags, 0, sizeof(m_WeekFlags));
	m_bMouseStat = 0;
	m_bEnabled = -1;
	m_penFrame = NULL;
}

CTimeSelectCtrl2::~CTimeSelectCtrl2(void)
{
}
// 设置周标记
void CTimeSelectCtrl2::SetWeekFlag(BYTE* pWeekFlag, int nSize)
{
	nSize = min(nSize, 21);
	for(int i = 0; i < nSize; i++)
	{
		for(int k = 0; k < 8; k++)
		{
			m_WeekFlags[i / 3][(i % 3) * 8 + k][0] = (0 != ((0x1 << k) & pWeekFlag[i]));
			m_WeekFlags[i / 3][(i % 3) * 8 + k][1] = (0 != ((0x1 << k) & pWeekFlag[i]));
			m_WeekFlags[i / 3][(i % 3) * 8 + k][2] = (0 != ((0x1 << k) & pWeekFlag[i]));
			m_WeekFlags[i / 3][(i % 3) * 8 + k][3] = (0 != ((0x1 << k) & pWeekFlag[i]));
		}
	}
	// 刷新
	if( IsWindow() )
	{
		Invalidate();
	}
}
// 设置周标记
void CTimeSelectCtrl2::SetWeekFlagEx(BYTE* pWeekFlag, int nSize)
{
	nSize = min(nSize, 84);
	for(int i = 0; i < nSize; i++)
	{
		for(int k = 0; k < 8; k++)
		{
			m_WeekFlags[i / 12][(i % 12) * 2 + k / 4][k % 4] = (0 != ((0x1 << k) & pWeekFlag[i]));
		}
	}
	// 刷新
	if( IsWindow() )
	{
		Invalidate();
	}
}
// 获取周标记
void CTimeSelectCtrl2::GetWeekFlag(BYTE* pWeekFlag, int nSize)
{
	nSize = min(nSize, 21);
	for(int i = 0; i < nSize; i++)
	{
		pWeekFlag[i] = 0;
		for(int k = 0; k < 8; k++)
		{
			pWeekFlag[i] |= ((m_WeekFlags[i / 3][(i % 3) * 8 + k][0])?0x1:0x0) << k;
			pWeekFlag[i] |= ((m_WeekFlags[i / 3][(i % 3) * 8 + k][1])?0x1:0x0) << k;
			pWeekFlag[i] |= ((m_WeekFlags[i / 3][(i % 3) * 8 + k][2])?0x1:0x0) << k;
			pWeekFlag[i] |= ((m_WeekFlags[i / 3][(i % 3) * 8 + k][3])?0x1:0x0) << k;
		}
	}
}
// 获取周标记
void CTimeSelectCtrl2::GetWeekFlagEx(BYTE* pWeekFlag, int nSize)
{
	nSize = min(nSize, 84);
	for(int i = 0; i < nSize; i++)
	{
		pWeekFlag[i] = 0;
		for(int k = 0; k < 8; k++)
		{
			pWeekFlag[i] |= ((m_WeekFlags[i / 12][(i % 12) * 2 + k / 4][k % 4])?0x1:0x0) << k;
		}
	}
}
// 获取颜色
COLORREF CTimeSelectCtrl2::GetRGBValue(short r, short g, short b, BOOL bGray /* = TRUE */)
{
	if(m_bEnabled || FALSE == bGray)
		return RGB(r, g, b);
	// 灰度算法 [r,g,b] = (r * 3 + g * 6 + g) / 10;
	r = g = b = (r * 3 + g * 6 + g) / 10;
	return RGB(r, g, b);
}
COLORREF CTimeSelectCtrl2::GetRGBValue(COLORREF col, BOOL bGray /* = TRUE */)
{
	if(m_bEnabled || FALSE == bGray)
		return col;

	short			r		= GetRValue(col);
	short			g		= GetGValue(col);
	short			b		= GetBValue(col);

	// 灰度算法 [r,g,b] = (r * 3 + g * 6 + g) / 10;
	r = g = b = (r * 3 + g * 6 + g) / 10;
	return RGB(r, g, b);
}
// 设置事件回调函数
void CTimeSelectCtrl2::SetEvent(UINT msg, HWND hRecv, UINT msgRecv)
{
	_Event		evt		= {0};

	evt.hWnd = hRecv;
	evt.msg = msgRecv;
	m_mapEvent[msg] = evt;
}
//////////////////////////////////////////////////////////////////////////
// 绘制一项
void CTimeSelectCtrl2::DrawItem(HDC hDC, LPRECT lpRect)
{
	POINT				pt		= {0};
	RECT				rt		= {0};

	if(NULL == m_penLine)
	{
		m_penLine = ::CreatePen(PS_SOLID, 1, GetRGBValue(0xD9, 0xE4, 0xE3));
	}
	if(NULL != m_penLine)
		::SelectObject(hDC, m_penLine);
	::SetBkMode(hDC, TRANSPARENT);
	// 绘制左上解
	GetItemRect(DR_NORTHWEST, hDC, lpRect, &rt);
	Draw_NorthWest(hDC, &rt);
	// 绘制顶部
	GetItemRect(DR_NORTH, hDC, lpRect, &rt);
	Draw_North(hDC, &rt);
	// 绘制左边
	GetItemRect(DR_WEST, hDC, lpRect, &rt);
	Draw_West(hDC, &rt);
	// 绘制主区域
	GetItemRect(DR_CENTER, hDC, lpRect, &rt);
	Draw_Center(hDC, &rt);
	// 绘制选区虚线框 
	Draw_SelRect(hDC, &m_rtMouseSel, lpRect);
	// 计算主体区大小
	GetItemRect(DR_BODY, hDC, lpRect, &rt);
	Draw_Frame(hDC, &rt);
}
// 绘制边框
void CTimeSelectCtrl2::Draw_Frame(HDC hDC, LPRECT lpRect)
{
	POINT		pt		= {0};

	if(NULL == m_penFrame)
	{
		m_penFrame = ::CreatePen(PS_SOLID, 1, GetRGBValue(0xD9, 0xE4, 0xE3));
	}
	if(NULL != m_penFrame)
	{
		::SelectObject(hDC, m_penFrame);
	}

	::MoveToEx(hDC, lpRect->left, lpRect->top, &pt);
	::LineTo(hDC, lpRect->right - 1, lpRect->top);
	::LineTo(hDC, lpRect->right - 1, lpRect->bottom - 1);
	::LineTo(hDC, lpRect->left, lpRect->bottom - 1);
	::LineTo(hDC, lpRect->left, lpRect->top);
}
// 绘制背景
void CTimeSelectCtrl2::Draw_Bk(HWND hWnd, HDC hDC)
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
// 绘制顶部背景色
void CTimeSelectCtrl2::Draw_North_Bk(HDC hDC, LPRECT lpRect)
{
	int					nHeight		= (lpRect->bottom - lpRect->top) / 2 + 1;
	BYTE				stepR		= (GetRValue(s_TopBk[1]) - GetRValue(s_TopBk[0])) / nHeight;
	BYTE				stepG		= (GetGValue(s_TopBk[1]) - GetGValue(s_TopBk[0])) / nHeight;
	BYTE				stepB		= (GetBValue(s_TopBk[1]) - GetBValue(s_TopBk[0])) / nHeight;

	// 画上部份
	for(int i = 0; i < nHeight; i++)
	{
		COLORREF		col			= GetRGBValue(stepR * i + GetRValue(s_TopBk[0])
			, stepG * i + GetGValue(s_TopBk[0])
			, stepB * i + GetBValue(s_TopBk[0]));
		RECT			rt;

		rt.left = lpRect->left;
		rt.right = lpRect->right;
		rt.top = lpRect->top + i;
		rt.bottom = rt.top + 1;
		::SetBkColor(hDC, col);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
	}
	// 画下部份
	for(int i = nHeight; i < (lpRect->bottom - lpRect->top); i++)
	{
		COLORREF		col			= GetRGBValue(GetRValue(s_TopBk[1]) - stepR * (i - nHeight)
			, GetGValue(s_TopBk[1]) - stepG * (i - nHeight)
			, GetBValue(s_TopBk[1]) - stepB * (i - nHeight));
		RECT			rt;

		rt.left = lpRect->left;
		rt.right = lpRect->right;
		rt.top = lpRect->top + i;
		rt.bottom = rt.top + 1;
		::SetBkColor(hDC, col);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
	}
}
// 绘制头部
void CTimeSelectCtrl2::Draw_NorthWest(HDC hDC, LPRECT lpRect)
{
	POINT		pt		= {0};

	Draw_North_Bk(hDC, lpRect);

	MoveToEx(hDC, lpRect->left, lpRect->bottom - 1, &pt);
	LineTo(hDC, lpRect->right - 1, lpRect->bottom - 1);
	LineTo(hDC, lpRect->right - 1, lpRect->top - 1);
}
// 绘制标题 
void CTimeSelectCtrl2::Draw_North(HDC hDC, LPRECT lpRect)
{
	POINT		pt				= {0};
	int			nWidth			= (lpRect->right - lpRect->left) / 24;
	WCHAR		szHour[4]		= {0};

	Draw_North_Bk(hDC, lpRect);

	MoveToEx(hDC, lpRect->left, lpRect->bottom - 1, &pt);
	LineTo(hDC, lpRect->right, lpRect->bottom - 1);
	for(int i = 1; i <= 24; i++)
	{
		RECT		rt		= {0};

		rt.left = lpRect->left + (i-1) * nWidth;
		rt.right = rt.left + nWidth - 1;
		rt.top = lpRect->top;
		rt.bottom = lpRect->bottom - 1;
		MoveToEx(hDC, lpRect->left + i * nWidth - 1, lpRect->top, &pt);
		LineTo(hDC, lpRect->left + i * nWidth - 1, lpRect->bottom);
		_itow(i - 1, szHour, 10);
		DrawTextW(hDC, szHour, (int)wcslen(szHour), &rt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
}
// 绘制左边
void CTimeSelectCtrl2::Draw_West(HDC hDC, LPRECT lpRect)
{
	POINT		pt				= {0};
	int			nHeight			= (lpRect->bottom - lpRect->top) / 7;
	
	MoveToEx(hDC, lpRect->right - 1, lpRect->top, &pt);
	LineTo(hDC, lpRect->right - 1, lpRect->bottom);
	for(int i = 1; i <= 7; i++)
	{
		RECT		rt		= {0};

		rt.left = lpRect->left;
		rt.right = lpRect->right - 1;
		rt.top = lpRect->top + (i - 1) * nHeight;
		rt.bottom = rt.top + nHeight - 1;
		// 绘制背景
		::SetBkColor(hDC, GetRGBValue(s_WeekBk[i - 1]));
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
		DrawTextW(hDC, &m_szWeek[i - 1], 1, &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		// 绘制边框
		MoveToEx(hDC, lpRect->left, lpRect->top + i * nHeight - 1, &pt);
		LineTo(hDC, lpRect->right, lpRect->top + i * nHeight - 1);
	}
}

// 绘制主区域
void CTimeSelectCtrl2::Draw_Center(HDC hDC, LPRECT lpRect)
{
	int			nWidth		= (lpRect->right - lpRect->left) / 24;
	int			nHeight		= (lpRect->bottom - lpRect->top) / 7;
	int			nm			= (nWidth - 1) / 4;
	POINT		pt;

	// 绘制主体区背景
	::SetBkColor(hDC, GetRGBValue(0xff, 0xff, 0xff));
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	// 开始绘制方格
	::SetBkColor(hDC, GetRGBValue(0x68, 0xD3, 0x73));
	for(int i = 0; i < 7; i++)
	{
		for(int k = 0; k < 24; k++)
		{
			RECT		rt		= {0};

			rt.left = lpRect->left + k * nWidth;
			rt.right = rt.left + nWidth;
			rt.top = lpRect->top + i * nHeight;
			rt.bottom = rt.top + nHeight;
			// 画边框
			MoveToEx(hDC, rt.left, rt.bottom - 1, &pt);
			LineTo(hDC, rt.right - 1, rt.bottom - 1);
			LineTo(hDC, rt.right - 1, rt.top - 1);
			if(false == m_WeekFlags[i][k][0]
				&& false == m_WeekFlags[i][k][1]
				&& false == m_WeekFlags[i][k][2]
				&& false == m_WeekFlags[i][k][3])
			{
				continue;
			}
			// 限制绘画区域
			rt.right -= 1;
			rt.bottom -= 1;
			// 画选中项, 选中项分四格
			for(int m = 0; m < 4; m++)
			{
				RECT		rtm			= rt;

				if(false == m_WeekFlags[i][k][m])
				{
					continue;
				}

				rtm.left = rt.left + m * nm;
				if(3 != m)
				{
					rtm.right = rtm.left + nm;
				}
				::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rtm, NULL, 0, NULL);
			}
		}
	}
}
// 绘制选区虚线框
void CTimeSelectCtrl2::Draw_SelRect(HDC hDC, LPRECT lpRect, LPCRECT lpWndRect)
{
	if(lpRect->left >= lpRect->right || lpRect->top >= lpRect->bottom)
	{
		return;
	}

	RECT			rt		= {0};
	BLENDFUNCTION	bf		= {0};
	RECT			rt2;

	rt.left = 0;
	rt.right = lpRect->right - lpRect->left;
	rt.top = lpWndRect->bottom + 5;
	rt.bottom = rt.top + lpRect->bottom - lpRect->top;
	rt2 = rt;
	rt2.left++;
	rt2.top++;
	rt2.right--;
	rt2.bottom--;

	::SetBkColor(hDC, RGB(0x00, 0x6D, 0xDB));
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
	::SetBkColor(hDC, RGB(0xA8, 0xCA, 0xEC));
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt2, NULL, 0, NULL);

	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 160;
	if(FALSE == ::AlphaBlend(hDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top,
		hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, bf))
	{
		ATLTRACE("AlphaBlend failed: %d\n", GetLastError());
	}
}

// 获取区域大小
void CTimeSelectCtrl2::GetItemRect(int nItem, HDC hDC, LPRECT lpRtClient, LPRECT lpRect)
{
	RECT			rtChar			= {0};
	RECT			rtCenter		= {0};
	int				nCharWidth, nCharHeight;
	int				nCenterWidth, nCenterHeight;

	// 获取字宽
	DrawTextW(hDC, m_szWeek, 1, &rtChar, DT_CALCRECT); 
	nCharWidth = rtChar.right - rtChar.left + 10;
	nCharHeight = rtChar.bottom - rtChar.top + 10;
	// 复制区域
	memcpy(lpRect, lpRtClient, sizeof(RECT));
	memcpy(&rtCenter, lpRtClient, sizeof(RECT));
	rtCenter.top += nCharHeight + 1;
	rtCenter.left += nCharWidth + 1;
	rtCenter.right -= 1;
	rtCenter.bottom -= 1;
	nCenterWidth = (rtCenter.right - rtCenter.left) / 24 * 24;
	nCenterHeight = (rtCenter.bottom - rtCenter.top) / 7 * 7;
	nCenterWidth = ((nCenterWidth / 24 - 1) / 4 * 4 + 1) * 24; // 保证每格都一样大小
	// 获取主区域大小
	if(DR_BODY == nItem)
	{
		lpRect->bottom = lpRect->top + nCharHeight + nCenterHeight + 1;
		lpRect->right = lpRect->left + nCharWidth + nCenterWidth + 1;
		return;
	}
	// 获取子区域大小
	lpRect->left += 1; // 边框
	lpRect->top += 1; // 边框
	lpRect->right -= 1;
	lpRect->bottom -= 1;
	if(DR_NORTHWEST == nItem)
	{
		lpRect->bottom = lpRect->top + nCharHeight;
		lpRect->right = lpRect->left + nCharWidth;
	}
	else if(DR_NORTH == nItem)
	{
		lpRect->bottom = lpRect->top + nCharHeight;
		lpRect->left += nCharWidth;
		lpRect->right = lpRect->left + nCenterWidth;
	}
	else if(DR_WEST == nItem)
	{
		lpRect->top += nCharHeight;
		lpRect->right = lpRect->left + nCharWidth;
		lpRect->bottom = lpRect->top + nCenterHeight;
	}
	else
	{
		lpRect->top += nCharHeight;
		lpRect->left += nCharWidth;
		lpRect->right = lpRect->left + nCenterWidth;
		lpRect->bottom = lpRect->top + nCenterHeight;
	}
}
//////////////////////////////////////////////////////////////////////////
// 测试鼠标所在区域
int CTimeSelectCtrl2::HitTest(IN POINT* pt, OUT RECT* lpRect)
{
	RECT			rtClient		= {0};
	RECT			rect			= {0};
	HDC				hDC				= GetDC();
	HFONT			hFont			= NULL;
	int				nRet			= -1;

	hFont = (HFONT)SelectObject(hDC, GetFont());
	GetClientRect(&rtClient);
	do
	{
		// 检测是不是在左上角
		GetItemRect(DR_NORTHWEST, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 0;
			*lpRect = rect;
			break;
		}
		// 检测是不是在上边
		GetItemRect(DR_NORTH, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 1;
			*lpRect = rect;
			break;
		}
		// 检测是否在左边
		GetItemRect(DR_WEST, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 2;
			*lpRect = rect;
			break;
		}
		// 检测是否在主体
		GetItemRect(DR_CENTER, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 3;
			*lpRect = rect;
			break;
		}
	}while(FALSE);
	// 退出时还原DC
	SelectObject(hDC, hFont);
	ReleaseDC(hDC);	

	return nRet;
}
// 测试选区区域
BOOL CTimeSelectCtrl2::HitSelectRect(IN POINT* ptBegin, IN POINT* ptEnd, OUT RECT* lpRect
									, OUT RECT* lpBody /* = NULL */)
{
	RECT		rtBody		= {0};

	if(3 != HitTest(ptBegin, &rtBody))
		return FALSE;

	if(ptEnd->x < rtBody.left)
		ptEnd->x = rtBody.left;
	else if(ptEnd->x >= rtBody.right)
		ptEnd->x = rtBody.right - 1;
	if(ptEnd->y < rtBody.top)
		ptEnd->y = rtBody.top;
	else if(ptEnd->y >= rtBody.bottom)
		ptEnd->y = rtBody.bottom - 1;
	lpRect->left = min(ptBegin->x, ptEnd->x);
	lpRect->right = max(ptBegin->x, ptEnd->x);
	lpRect->top = min(ptBegin->y, ptEnd->y);
	lpRect->bottom = max(ptBegin->y, ptEnd->y);
	if(NULL != lpBody)
		*lpBody = rtBody;
	return TRUE;
}
// 反转一项
void CTimeSelectCtrl2::ReverseItemSel(int nR1, int nL1, int nR2, int nL2)
{
	int				nAllNoSel		= 0x0;
	int				nAllSel			= 0x0;
	bool			bSel			= true;

	for(int i = nR1; i <= nR2; i++)
	{
		for(int k = nL1; k <= nL2; k++)
		{
			if(m_WeekFlags[k][i][0]
			|| m_WeekFlags[k][i][1]
			|| m_WeekFlags[k][i][2]
			|| m_WeekFlags[k][i][3])
			{
				nAllNoSel |= 0x1;

			}
			else
			{
				nAllSel |= 0x1;
			}
		}
	}
	if(0x1 == nAllNoSel && 0x0 == nAllSel)
	{
		bSel = false;
	}
	// 设置
	for(int i = nR1; i <= nR2; i++)
	{
		for(int k = nL1; k <= nL2; k++)
		{
			m_WeekFlags[k][i][0] = bSel;
			m_WeekFlags[k][i][1] = bSel;
			m_WeekFlags[k][i][2] = bSel;
			m_WeekFlags[k][i][3] = bSel;
		}
	}
}
// 单击一项处理
void CTimeSelectCtrl2::OnClickOneItem(POINT* pt)
{
	RECT		rect		= {0};
	int			nItem		= -1;

	nItem = HitTest(pt, &rect);
	if(-1 == nItem)
		return;
	switch(nItem)
	{
	case 0:  // 左上角被单击
		OnClick_TopLeft();
		break;
	case 1:	// 上边被单击
		OnClick_Top(pt, &rect);
		break;
	case 2:	// 左边被单击
		OnClick_Left(pt, &rect);
		break;
	}
}
// 单击左上角处理
void CTimeSelectCtrl2::OnClick_TopLeft()
{
	ReverseItemSel(0, 0, 23, 6);
	Invalidate();
}
// 选区在顶部菜单处
void CTimeSelectCtrl2::OnClick_Top(POINT* pt, RECT* lpRect)
{
	int			nWidth			= (lpRect->right - lpRect->left) / 24;
	int			nSel			= -1;

	nSel = (pt->x - lpRect->left) / nWidth;
	ReverseItemSel(nSel, 0, nSel, 6);
	Invalidate();
}
// 左边一行被先择
void CTimeSelectCtrl2::OnClick_Left(POINT* pt, RECT* lpRect)
{
	int			nHeight		= (lpRect->bottom - lpRect->top) / 7;
	int			nSel		= -1;

	nSel = (pt->y - lpRect->top) / nHeight;
	ReverseItemSel(0, nSel, 23, nSel);
	Invalidate();
}
// 单击主体区域
void CTimeSelectCtrl2::OnClickBodyItem(POINT* ptBegin, POINT* ptEnd)
{
	RECT			rtSel		= {0};
	RECT			rtBody		= {0};

	// 对选区清0
	memset(&m_rtMouseSel, 0, sizeof(m_rtMouseSel));
	// 获取选区的项
	if(FALSE == HitSelectRect(ptBegin, ptEnd, &rtSel, &rtBody))
		return;
	// 获取选区内的项的行列式
	int				nL1, nL2, nR1, nR2;
	int				nWidth		= (rtBody.right - rtBody.left) / 24;
	int				nHeight		= (rtBody.bottom - rtBody.top) / 7;

	nR1 = (rtSel.left - rtBody.left) / nWidth;
	nR2 = (rtSel.right - rtBody.left) / nWidth;
	nL1 = (rtSel.top - rtBody.top) / nHeight;
	nL2 = (rtSel.bottom - rtBody.top) / nHeight;
	
	ReverseItemSel(nR1, nL1, nR2, nL2);
	Invalidate();
}
//////////////////////////////////////////////////////////////////////////
// 绘制消息
LRESULT CTimeSelectCtrl2::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	PAINTSTRUCT			paint			= {0};
	HDC					hDC				= NULL;
	RECT				rect			= {0};
	HDC					hMemDC			= NULL;
	HBITMAP				hBitmap			= NULL;
	int					nSaveDC			= 0;

	m_bEnabled = IsWindowEnabled();
	bHandled = TRUE;
	hDC = BeginPaint(&paint);
	GetClientRect(&rect);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, 
		(rect.bottom - rect.top) * 2); // 高度加12， 以画非显示图形
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

// 鼠标按下消息
LRESULT CTimeSelectCtrl2::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	int		nItem		= -1;
	RECT	rect		= {0};
	POINT	pt			= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	bHandled = TRUE;
	nItem = HitTest(&pt, &rect);
	if(0x3 == nItem)
	{
		m_bMouseStat = 0x2;
		m_ptMouseDown = pt;
	}
	else
	{
		m_bMouseStat = 0x1;
	}
	::SetCapture(m_hWnd);

	return 0;
}
// 鼠标弹起
LRESULT CTimeSelectCtrl2::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	POINT			pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	bHandled = TRUE;
	::ReleaseCapture();
	if(0x1 == m_bMouseStat)
	{
		// 单击其中一个单元
		OnClickOneItem(&pt);
	}
	else if(0x2 == m_bMouseStat)
	{
		// 单击主体区域
		OnClickBodyItem(&m_ptMouseDown, &pt);
	}

	m_bMouseStat = 0;
	// 通知事件改变
	std::map<UINT, _Event>::iterator	item;

	item = m_mapEvent.find(WM_LBUTTONUP);
	if(item != m_mapEvent.end())
	{
		::SendMessage(item->second.hWnd, item->second.msg, 0, 0);
	}
	
	return 0;
}
// 鼠标移动
LRESULT CTimeSelectCtrl2::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT			pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	if(0x2 == m_bMouseStat)
	{
		RECT		rtBody		= {0};

		HitSelectRect(&m_ptMouseDown, &pt, &m_rtMouseSel, &rtBody);
		Invalidate();
	}

	return 0;
}

LRESULT CTimeSelectCtrl2::OnEnable(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	Invalidate(TRUE);
	return 0;
}

// 鼠标弹起事件
LRESULT CTimeSelectCtrl2::OnRButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT			rtCenter	= {0};
	POINT			pt			= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	std::map<UINT, _Event>::iterator	item;

	if(DR_CENTER != HitTest(&pt, &rtCenter))
	{
		return 0;
	}
	item = m_mapEvent.find(WM_RBUTTONUP);
	if(item == m_mapEvent.end())
	{
		return 0;
	}
	// 将事件发送到上层
	int			nHour		= 0;
	int			nL, nT;
	int			nW, nH;
	
	nW = (rtCenter.right - rtCenter.left) / 24;
	nH = (rtCenter.bottom - rtCenter.top) / 7;
	nL = pt.x - rtCenter.left;
	nT = pt.y - rtCenter.top;
	nHour = nL / nW + nT / nH * 24;

	::ClientToScreen(m_hWnd, &pt);
	::ScreenToClient(item->second.hWnd, &pt);
	::SendMessage(item->second.hWnd, item->second.msg, nHour, MAKELPARAM(pt.x, pt.y));
	return 0;
}