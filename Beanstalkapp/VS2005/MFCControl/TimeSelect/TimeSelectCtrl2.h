#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <map>

class CTimeSelectCtrl2 : public CWindowImpl<CTimeSelectCtrl2>
{
public:
	CTimeSelectCtrl2(void);
	~CTimeSelectCtrl2(void);

public:
	BEGIN_MSG_MAP(CTimeSelectCtrl2)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_ENABLE, OnEnable)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
	END_MSG_MAP()

public:
	void		SetWeekFlag(BYTE* pWeekFlag, int nSize);
	void		SetWeekFlagEx(BYTE* pWeekFlag, int nSize);
	void		GetWeekFlag(BYTE* pWeekFlag, int nSize);
	void		GetWeekFlagEx(BYTE* pWeekFlag, int nSize);
	COLORREF	GetRGBValue(short r, short g, short b, BOOL bGray = TRUE);
	COLORREF	GetRGBValue(COLORREF col, BOOL bGray = TRUE);
	void		SetEvent(UINT msg, HWND hRecv, UINT msgRecv);

private:
	HPEN		m_penLine;
	HPEN		m_penFrame;
	WCHAR		m_szWeek[8];
	BYTE		m_bMouseStat;
	bool		m_WeekFlags[7][24][4];
	POINT		m_ptMouseDown;
	RECT		m_rtMouseSel;
	BOOL		m_bEnabled;

	struct _Event{
		HWND	hWnd;
		UINT	msg;
	};

	std::map<UINT, _Event>	m_mapEvent;
protected:
	void		DrawItem(HDC hDC, LPRECT lpRect);
	void		Draw_Frame(HDC hDC, LPRECT lpRect);
	void		Draw_Bk(HWND hWnd, HDC hDC);
	void		Draw_North_Bk(HDC hDC, LPRECT lpRect);
	void		Draw_NorthWest(HDC hDC, LPRECT lpRect);
	void		Draw_North(HDC hDC, LPRECT lpRect);
	void		Draw_West(HDC hDC, LPRECT lpRect);
	void		Draw_Center(HDC hDC, LPRECT lpRect);
	void		Draw_SelRect(HDC hDC, LPRECT lpRect, LPCRECT lpWndRect);
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
	LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

