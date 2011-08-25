#include "StdAfx.h"
#include "TimeSelectCtrl.h"
// ������ڱ���ɫ
COLORREF CTimeSelectCtrl::s_WeekBk[7] = {
	RGB(0xDD, 0xF5, 0xA6), RGB(0xEB, 0xC2, 0xAF), RGB(0xC0, 0xCA, 0xF7)
	, RGB(0xA8, 0xE9, 0xCF), RGB(0xD8, 0xC1, 0xEE), RGB(0x7E, 0xD3, 0xD5)
	, RGB(0xEE, 0x9B, 0xA2)
};
// ��������ɫ�͸�����
COLORREF CTimeSelectCtrl::s_TopBk[2] = {
	RGB(0xD7, 0xEE, 0xFC), RGB(0x97, 0xD4, 0xF3)
};
// �Ҷ��㷨 [R,G,B] = (r * 3 + g * 6 + g) / 10;

CTimeSelectCtrl::CTimeSelectCtrl(void)
{
	m_penLine = NULL;
	wcscpy(m_szWeek, L"��һ����������");
	memset(m_WeekFlags, 0, sizeof(m_WeekFlags));
	m_bMouseStat = 0;
	m_bEnabled = -1;
}

CTimeSelectCtrl::~CTimeSelectCtrl(void)
{
}
// �����ܱ��
void CTimeSelectCtrl::SetWeekFlag(BYTE* pWeekFlag, int nSize)
{
	nSize = min(nSize, 21);
	for(int i = 0; i < nSize; i++)
	{
		for(int k = 0; k < 8; k++)
		{
			m_WeekFlags[i / 3][(i % 3) * 8 + k] = (0x1 << k) & pWeekFlag[i];
		}
	}
	if(IsWindow())
		Invalidate();
}
// ��ȡ�ܱ��
void CTimeSelectCtrl::GetWeekFlag(BYTE* pWeekFlag, int nSize)
{
	nSize = min(nSize, 21);
	for(int i = 0; i < nSize; i++)
	{
		pWeekFlag[i] = 0;
		for(int k = 0; k < 8; k++)
		{
			pWeekFlag[i] |= ((m_WeekFlags[i / 3][(i % 3) * 8 + k])?0x1:0x0) << k;
		}
	}
}
// ��ȡ��ɫ
COLORREF CTimeSelectCtrl::GetRGBValue(short r, short g, short b, BOOL bGray /* = TRUE */)
{
	if(m_bEnabled || FALSE == bGray)
		return RGB(r, g, b);
	// �Ҷ��㷨 [r,g,b] = (r * 3 + g * 6 + g) / 10;
	r = g = b = (r * 3 + g * 6 + g) / 10;
	return RGB(r, g, b);
}
COLORREF CTimeSelectCtrl::GetRGBValue(COLORREF col, BOOL bGray /* = TRUE */)
{
	if(m_bEnabled || FALSE == bGray)
		return col;

	short			r		= GetRValue(col);
	short			g		= GetGValue(col);
	short			b		= GetBValue(col);

	// �Ҷ��㷨 [r,g,b] = (r * 3 + g * 6 + g) / 10;
	r = g = b = (r * 3 + g * 6 + g) / 10;
	return RGB(r, g, b);
}
//////////////////////////////////////////////////////////////////////////
// ����һ��
void CTimeSelectCtrl::DrawItem(HDC hDC, LPRECT lpRect)
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

	GetItemRect(0, hDC, lpRect, &rt);
	Draw_LeftTop(hDC, &rt);
	GetItemRect(1, hDC, lpRect, &rt);
	Draw_Top(hDC, &rt);
	GetItemRect(2, hDC, lpRect, &rt);
	Draw_Left(hDC, &rt);
	GetItemRect(3, hDC, lpRect, &rt);
	Draw_Body(hDC, &rt);
}
// ���ƶ�������ɫ
void CTimeSelectCtrl::Draw_Top_Bk(HDC hDC, LPRECT lpRect)
{
	int					nHeight		= (lpRect->bottom - lpRect->top) / 2 + 1;
	BYTE				stepR		= (GetRValue(s_TopBk[1]) - GetRValue(s_TopBk[0])) / nHeight;
	BYTE				stepG		= (GetGValue(s_TopBk[1]) - GetGValue(s_TopBk[0])) / nHeight;
	BYTE				stepB		= (GetBValue(s_TopBk[1]) - GetBValue(s_TopBk[0])) / nHeight;

	// ���ϲ���
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
	// ���²���
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
// ����ͷ��
void CTimeSelectCtrl::Draw_LeftTop(HDC hDC, LPRECT lpRect)
{
	POINT		pt		= {0};

	Draw_Top_Bk(hDC, lpRect);

	MoveToEx(hDC, lpRect->left, lpRect->bottom - 1, &pt);
	LineTo(hDC, lpRect->right - 1, lpRect->bottom - 1);
	LineTo(hDC, lpRect->right - 1, lpRect->top - 1);
}
// ���Ʊ��� 
void CTimeSelectCtrl::Draw_Top(HDC hDC, LPRECT lpRect)
{
	POINT		pt				= {0};
	int			nWidth			= (lpRect->right - lpRect->left) / 24 + 1;
	WCHAR		szHour[4]		= {0};

	Draw_Top_Bk(hDC, lpRect);

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
		DrawTextW(hDC, szHour, wcslen(szHour), &rt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
}
// �������
void CTimeSelectCtrl::Draw_Left(HDC hDC, LPRECT lpRect)
{
	POINT		pt				= {0};
	int			nHeight			= (lpRect->bottom - lpRect->top) / 7;
	
	MoveToEx(hDC, lpRect->right - 1, lpRect->top, &pt);
	LineTo(hDC, lpRect->right - 1, lpRect->bottom + 1);
	for(int i = 1; i <= 7; i++)
	{
		RECT		rt		= {0};

		rt.left = lpRect->left;
		rt.right = lpRect->right - 1;
		rt.top = lpRect->top + (i - 1) * nHeight;
		rt.bottom = rt.top + nHeight - 1;
		// ���Ʊ���
		::SetBkColor(hDC, GetRGBValue(s_WeekBk[i - 1]));
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
		// ���Ʊ߿�
		MoveToEx(hDC, lpRect->left, lpRect->top + i * nHeight - 1, &pt);
		LineTo(hDC, lpRect->right, lpRect->top + i * nHeight - 1);
		DrawTextW(hDC, &m_szWeek[i - 1], 1, &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

// ����������
void CTimeSelectCtrl::Draw_Body(HDC hDC, LPRECT lpRect)
{
	int			nWidth		= (lpRect->right - lpRect->left) / 24 + 1;
	int			nHeight		= (lpRect->bottom - lpRect->top) / 7;
	POINT		pt;

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
			// ���߿�
			MoveToEx(hDC, rt.left, rt.bottom - 1, &pt);
			LineTo(hDC, rt.right - 1, rt.bottom - 1);
			LineTo(hDC, rt.right - 1, rt.top - 1);
			// ��ѡ����
			if(false == m_WeekFlags[i][k])
				continue;
			rt.left += 1;
			rt.top += 1;
			rt.right -= 1;
			rt.bottom -= 1;
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
		}
	}
	// ����ѡ�����߿� 
	Draw_SelRect(hDC, &m_rtMouseSel);
}
// ����ѡ�����߿�
void CTimeSelectCtrl::Draw_SelRect(HDC hDC, LPRECT lpRect)
{
	::SetBkColor(hDC, GetRGBValue(0xff, 0xff, 0xff));
	::DrawFocusRect(hDC, lpRect);
}

// ��ȡ�����С
void CTimeSelectCtrl::GetItemRect(int nItem, HDC hDC, LPRECT lpRtClient, LPRECT lpRect)
{
	RECT			rtChar			= {0};

	DrawTextW(hDC, m_szWeek, 1, &rtChar, DT_CALCRECT); 

	memcpy(lpRect, lpRtClient, sizeof(RECT));
	if(0 == nItem)
	{
		lpRect->bottom = lpRect->top + rtChar.bottom - rtChar.top + 10;
		lpRect->right = lpRect->left + rtChar.right - rtChar.left + 10;
	}
	else if(1 == nItem)
	{
		lpRect->bottom = lpRect->top + rtChar.bottom - rtChar.top + 10;
		lpRect->left += rtChar.right - rtChar.left + 10;
	}
	else if(2 == nItem)
	{
		lpRect->top += rtChar.bottom - rtChar.top + 10;
		lpRect->right = lpRect->left + rtChar.right - rtChar.left + 10;
	}
	else
	{
		lpRect->top += rtChar.bottom - rtChar.top + 10;
		lpRect->left += lpRect->left + rtChar.right - rtChar.left + 10;
	}
}
//////////////////////////////////////////////////////////////////////////
// ���������������
int CTimeSelectCtrl::HitTest(IN POINT* pt, OUT RECT* lpRect)
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
		// ����ǲ��������Ͻ�
		GetItemRect(0, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 0;
			*lpRect = rect;
			break;
		}
		// ����ǲ������ϱ�
		GetItemRect(1, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 1;
			*lpRect = rect;
			break;
		}
		// ����Ƿ������
		GetItemRect(2, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 2;
			*lpRect = rect;
			break;
		}
		// ����Ƿ�������
		GetItemRect(3, hDC, &rtClient, &rect);
		if(pt->x > rect.left && pt->x < rect.right && pt->y > rect.top && pt->y < rect.bottom)
		{
			nRet = 3;
			*lpRect = rect;
			break;
		}
	}while(FALSE);
	// �˳�ʱ��ԭDC
	SelectObject(hDC, hFont);
	ReleaseDC(hDC);	

	return nRet;
}
// ����ѡ������
BOOL CTimeSelectCtrl::HitSelectRect(IN POINT* ptBegin, IN POINT* ptEnd, OUT RECT* lpRect
									, OUT RECT* lpBody /* = NULL */)
{
	RECT		rtBody		= {0};

	if(3 != HitTest(ptBegin, &rtBody))
		return FALSE;

	if(ptEnd->x < rtBody.left)
		ptEnd->x = rtBody.left;
	else if(ptEnd->x > rtBody.right)
		ptEnd->x = rtBody.right;
	if(ptEnd->y < rtBody.top)
		ptEnd->y = rtBody.top;
	else if(ptEnd->y > rtBody.bottom)
		ptEnd->y = rtBody.bottom;
	lpRect->left = min(ptBegin->x, ptEnd->x);
	lpRect->right = max(ptBegin->x, ptEnd->x);
	lpRect->top = min(ptBegin->y, ptEnd->y);
	lpRect->bottom = max(ptBegin->y, ptEnd->y);
	if(NULL != lpBody)
		*lpBody = rtBody;
	return TRUE;
}
// ��תһ��
void CTimeSelectCtrl::ReverseItemSel(int nR1, int nL1, int nR2, int nL2)
{
	int				nAllNoSel		= 0x0;
	int				nAllSel			= 0x0;
	bool			bSel			= true;

	for(int i = nR1; i <= nR2; i++)
	{
		for(int k = nL1; k <= nL2; k++)
		{
			if(m_WeekFlags[k][i])
				nAllNoSel |= 0x1;
			else
				nAllSel |= 0x1;
		}
	}
	if(0x1 == nAllNoSel && 0x0 == nAllSel)
		bSel = false;
	// ����
	for(int i = nR1; i <= nR2; i++)
	{
		for(int k = nL1; k <= nL2; k++)
		{
			m_WeekFlags[k][i] = bSel;
		}
	}
}
// ����һ���
void CTimeSelectCtrl::OnClickOneItem(POINT* pt)
{
	RECT		rect		= {0};
	int			nItem		= -1;

	nItem = HitTest(pt, &rect);
	if(-1 == nItem)
		return;
	switch(nItem)
	{
	case 0:  // ���ϽǱ�����
		OnClick_TopLeft();
		break;
	case 1:	// �ϱ߱�����
		OnClick_Top(pt, &rect);
		break;
	case 2:	// ��߱�����
		OnClick_Left(pt, &rect);
		break;
	}
}
// �������ϽǴ���
void CTimeSelectCtrl::OnClick_TopLeft()
{
	ReverseItemSel(0, 0, 23, 6);
	Invalidate();
}
// ѡ���ڶ����˵���
void CTimeSelectCtrl::OnClick_Top(POINT* pt, RECT* lpRect)
{
	int			nWidth			= (lpRect->right - lpRect->left) / 24 + 1;
	int			nSel			= -1;

	nSel = (pt->x - lpRect->left) / nWidth;
	ReverseItemSel(nSel, 0, nSel, 6);
	Invalidate();
}
// ���һ�б�����
void CTimeSelectCtrl::OnClick_Left(POINT* pt, RECT* lpRect)
{
	int			nHeight		= (lpRect->bottom - lpRect->top) / 7;
	int			nSel		= -1;

	nSel = (pt->y - lpRect->top) / nHeight;
	ReverseItemSel(0, nSel, 23, nSel);
	Invalidate();
}
// ������������
void CTimeSelectCtrl::OnClickBodyItem(POINT* ptBegin, POINT* ptEnd)
{
	RECT			rtSel		= {0};
	RECT			rtBody		= {0};

	// ��ѡ����0
	memset(&m_rtMouseSel, 0, sizeof(m_rtMouseSel));
	// ��ȡѡ������
	if(FALSE == HitSelectRect(ptBegin, ptEnd, &rtSel, &rtBody))
		return;
	// ��ȡѡ���ڵ��������ʽ
	int				nL1, nL2, nR1, nR2;
	int				nWidth		= (rtBody.right - rtBody.left) / 24 + 1;
	int				nHeight		= (rtBody.bottom - rtBody.top) / 7;

	nR1 = (rtSel.left - rtBody.left) / nWidth;
	nR2 = (rtSel.right - rtBody.left) / nWidth;
	nL1 = (rtSel.top - rtBody.top) / nHeight;
	nL2 = (rtSel.bottom - rtBody.top) / nHeight;
	
	ReverseItemSel(nR1, nL1, nR2, nL2);
	InvalidateRect(&rtBody);
}
//////////////////////////////////////////////////////////////////////////
// ������Ϣ
LRESULT CTimeSelectCtrl::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
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
	hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
	nSaveDC = SaveDC(hMemDC);

	::SelectObject(hMemDC, hBitmap);
	::SelectObject(hMemDC, GetFont());
	::SetBkColor(hMemDC, GetRGBValue(0xff, 0xff, 0xff));
	::ExtTextOut(hMemDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

	// ��������
	DrawItem(hMemDC, &rect);
	// �����ڴ�DC
	::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
		, hMemDC, 0, 0, SRCCOPY);

	RestoreDC(hDC, nSaveDC);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	EndPaint(&paint);
	return 0;
}

// ��갴����Ϣ
LRESULT CTimeSelectCtrl::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
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
// ��굯��
LRESULT CTimeSelectCtrl::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	POINT			pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	bHandled = TRUE;
	::ReleaseCapture();
	if(0x1 == m_bMouseStat)
	{
		// ��������һ����Ԫ
		OnClickOneItem(&pt);
	}
	else if(0x2 == m_bMouseStat)
	{
		// ������������
		OnClickBodyItem(&m_ptMouseDown, &pt);
	}

	m_bMouseStat = 0;
	return 0;
}
// ����ƶ�
LRESULT CTimeSelectCtrl::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT			pt		= {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	if(0x2 == m_bMouseStat)
	{
		RECT		rtBody		= {0};

		HitSelectRect(&m_ptMouseDown, &pt, &m_rtMouseSel, &rtBody);
		InvalidateRect(&rtBody);
	}

	return 0;
}

LRESULT CTimeSelectCtrl::OnEnable(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	Invalidate(TRUE);
	return 0;
}
