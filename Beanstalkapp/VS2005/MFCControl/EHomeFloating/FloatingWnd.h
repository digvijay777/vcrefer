#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <gdiplus.h>

class CFloatingWnd : public CWindowImpl<CFloatingWnd>
{
public:
	CFloatingWnd(Gdiplus::Image* pImage, int nSplit);
	virtual ~CFloatingWnd(void);

public:
	// CWindow�ķ���
	static DWORD GetWndStyle(DWORD dwStyle);		// ������ʽ
	static DWORD GetWndExStyle(DWORD dwStyle);		// ������ʽ
	BOOL UpdateWindow();
	// ��Ϣѭ��
	BEGIN_MSG_MAP(CFloatingWnd)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()

protected:
	Gdiplus::Image*			m_pBkImage;
	SIZE					m_size;
	RECT					m_rtClient;

public:
	int						m_nBkImageIndex;
	WCHAR					m_szShowText[128];

private:
	void	UpdateLayeredImageWindow(HDC hDC);

public:
	void	SetShowText(LPCWSTR lpFmt, ...);
	void	SetClientRect(int x, int y, int w, int h);

public:
	virtual	RECT		GetInitRect();
	virtual void		OnLayerdPrint(HDC hDC, LPRECT lpRect);
public:
	LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
