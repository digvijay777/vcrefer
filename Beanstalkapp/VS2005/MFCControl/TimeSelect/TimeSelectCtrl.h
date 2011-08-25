#pragma once
#include <atlbase.h>
#include <atlwin.h>

class CTimeSelectCtrl : public CWindowImpl<CTimeSelectCtrl>
{
public:
	CTimeSelectCtrl(void);
	~CTimeSelectCtrl(void);
public:
	BEGIN_MSG_MAP(CTimeSelectCtrl)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_ENABLE, OnEnable)
	END_MSG_MAP()

public:
	void		SetWeekFlag(BYTE* pWeekFlag, int nSize);
	void		GetWeekFlag(BYTE* pWeekFlag, int nSize);
	COLORREF	GetRGBValue(short r, short g, short b, BOOL bGray = TRUE);
	COLORREF	GetRGBValue(COLORREF col, BOOL bGray = TRUE);
private:
	HPEN		m_penLine;
	WCHAR		m_szWeek[8];
	BYTE		m_bMouseStat;
	bool		m_WeekFlags[7][24];
	POINT		m_ptMouseDown;
	RECT		m_rtMouseSel;
	BOOL		m_bEnabled;
protected:
	void		DrawItem(HDC hDC, LPRECT lpRect);
	void		Draw_Top_Bk(HDC hDC, LPRECT lpRect);
	void		Draw_LeftTop(HDC hDC, LPRECT lpRect);
	void		Draw_Top(HDC hDC, LPRECT lpRect);
	void		Draw_Left(HDC hDC, LPRECT lpRect);
	void		Draw_Body(HDC hDC, LPRECT lpRect);
	void		Draw_SelRect(HDC hDC, LPRECT lpRect);
	void		GetItemRect(int nItem, HDC hDC, LPRECT lpRtClient, LPRECT lpRect);
protected:
	int			HitTest(IN POINT* pt, OUT RECT* lpRect);
	BOOL		HitSelectRect(IN POINT* ptBegin, IN POINT* ptEnd, OUT RECT* lpRect, OUT RECT* lpBody = NULL);
	void		ReverseItemSel(int nR1, int nL1, int nR2, int nL2);
	void		OnClickOneItem(POINT* pt);
	void		OnClick_TopLeft();
	void		OnClick_Top(POINT* pt, RECT* lpRect);
	void		OnClick_Left(POINT* pt, RECT* lpRect);
	void		OnClickBodyItem(POINT* ptBegin, POINT* ptEnd);
public:
	static COLORREF		s_WeekBk[7];
	static COLORREF		s_TopBk[2];
public:
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEnable(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

