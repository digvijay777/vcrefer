#pragma once
#include <atlbase.h>
#include <atlwin.h>

// CTimeSelectCtrl3

class CTimeSelectCtrl3 : public CWindowImpl<CTimeSelectCtrl3>
{
public:
	CTimeSelectCtrl3(void);
	~CTimeSelectCtrl3(void);

public:
	BEGIN_MSG_MAP(CTimeSelectCtrl3)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()

private:
	int			m_nSplit;
	bool*		m_pData;
	WCHAR*		m_pText;
	int			m_nButtonDown;

public:
	void		SetData(bool* pData, int nCount, WCHAR* pText = NULL);
	int			HitTestItem(POINT pt);

protected:
	void		DrawItem(HDC hDC, LPRECT lpRect);
	void		Draw_Bk(HWND hWnd, HDC hDC);

public:
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

