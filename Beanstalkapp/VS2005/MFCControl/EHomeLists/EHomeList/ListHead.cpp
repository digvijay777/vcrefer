#include "stdafx.h"
#include "ListHead.h"

CListHead::CListHead()
{

}

CListHead::~CListHead()
{

}

BEGIN_MESSAGE_MAP(CListHead, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()

void CListHead::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	// 画背景
	CRect wndRt, filRt;
	BYTE byRed=253, byGrn=253, byBlue=253;

	GetWindowRect(&wndRt);
	ScreenToClient(&wndRt);
	filRt.CopyRect(&wndRt);

	for (; filRt.top<wndRt.bottom; )
	{
		filRt.bottom = filRt.top + 1;
		CBrush brush(RGB(byRed, byGrn, byBlue));
		dc.FillRect(&filRt, &brush);

		filRt.top += 1;
		byRed -= 2;
		byGrn -= 2;
		byBlue-= 2;
	}
	
	// 画标题和分隔符
	CFont font;
	LOGFONT lf;

	memset(&lf, 0, sizeof(LOGFONT));		// zero out structure
	lf.lfHeight = 15;						// request a 15-pixel-height font
	lf.lfWeight = FW_NORMAL;				// request font weight
	_tcscpy(lf.lfFaceName, TEXT("Arial"));	// request a face name "Arial"
	VERIFY(font.CreateFontIndirect(&lf));	// create the font
	
	CFont* def_font = dc.SelectObject(&font);

	int		nCount = GetItemCount();
	HDITEM	hdi;
	TCHAR	lpBuffer[64];
	int		iDefl;
	TEXTMETRIC tm;
	
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 64;
	
	CPen nPen(PS_SOLID, 1, RGB(0xbd, 0xbd, 0xbd));
	CPen *oldPen = dc.SelectObject(&nPen);
	
	dc.GetTextMetrics(&tm);
	iDefl = (wndRt.Height() - tm.tmHeight ) / 2;

	for (int i=0; i < nCount; i++)
	{
		GetItem(i, &hdi);
		GetItemRect(i, &filRt);
		filRt.DeflateRect(0, iDefl, 0, iDefl);

		dc.MoveTo(filRt.right-1, filRt.top);
		dc.LineTo(filRt.right-1, filRt.bottom);
		
		dc.DrawText(lpBuffer, &filRt, DT_CENTER);
	}

	dc.SelectObject(def_font);	
	dc.SelectObject(oldPen);
	font.DeleteObject();
	nPen.DeleteObject();
}

LRESULT CListHead::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam;
	RECT *prc = hdl.prc;
	WINDOWPOS *pwpos = hdl.pwpos;
	int nHeight = (int)(pwpos->cy * 1.3); //改变高度
	pwpos->cy = nHeight;
	prc->top = nHeight;

	return lResult;
}
