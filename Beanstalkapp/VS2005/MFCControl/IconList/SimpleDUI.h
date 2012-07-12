#pragma once
#include <string>

class CSimpleDUIBase;
typedef struct _DUI_PANEL_ROOT
{
	HWND					hWnd;
	CSimpleDUIBase*			trackUI;
	UINT					trackMsg;
	CSimpleDUIBase*			captureUI;
	CSimpleDUIBase*			focusUI;
} DUI_PANEL_ROOT, *PDUI_PANEL_ROOT;
/*
 *	Direct窗体基类
 */
class CSimpleDUIBase
{
public:
	CSimpleDUIBase(CSimpleDUIBase* parent);
	virtual ~CSimpleDUIBase();

private:
	CSimpleDUIBase*		m_parent;
	CSimpleDUIBase*		m_child;
	CSimpleDUIBase*		m_brother;
	RECT				m_rect;

protected:
	BOOL				m_isVisible;

protected:
	void	DeleteDUI(CSimpleDUIBase* pBase);
	CSimpleDUIBase*		GetChild();

public:
	BOOL	DispatchEvent(POINT pt, UINT nMsg, 
		WPARAM wParam, LPARAM lParam);
	void	TranslateTrackEvent();
	void	MoveUI(LPRECT lpRect);
	void	Draw(HDC hDC, LPRECT lpRect);
	BOOL	IsVisible();
	void	ShowUI(BOOL bShow);
	void	SetCapture();
	void	ReleaseCapture();
	void	SetFocus();
	void	KillFocus();
	void	TrackEvent(UINT nMsg);
	void	RectToPanel(RECT* rect);
	void	GetUIRect(RECT* rect);

public:
	virtual void				Invalidate(LPRECT lpRect = NULL);
	virtual PDUI_PANEL_ROOT		GetPaneRoot();
	virtual BOOL				OnEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void				OnDraw(HDC hDC, LPRECT lpRect) = 0;

public:
	static void						MergerRect(RECT* des, LPRECT rt1, LPRECT rt2);
};

/*
 *	主面板
 */
class CSimpleDUIPanel : public CSimpleDUIBase
{
public:
	CSimpleDUIPanel();
	virtual ~CSimpleDUIPanel();

private:
	DUI_PANEL_ROOT				m_panelRoot;
	BOOL						m_bTrackEvent;

public:
	BOOL			TranslateEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	void			Paint(HWND hWnd, HDC hDC);

public:
	virtual PDUI_PANEL_ROOT		GetPaneRoot();
	virtual void				EraseBkgnd(HDC hDC, HWND hWnd);
	virtual void				Invalidate(LPRECT lpRect);
	virtual void				OnDraw(HDC hDC, LPRECT lpRect);
};

/*
 *	绘制文本
 */
class CSimpleDUIText : public CSimpleDUIBase
{
public:
	CSimpleDUIText(LPCWSTR lpText, CSimpleDUIBase* parent);
	virtual ~CSimpleDUIText();

private:
	std::wstring	m_text;

public:
	virtual		void	OnDraw(HDC hDC, LPRECT lpRect);

public:
	void	SetText(LPCWSTR lpText);
};

/*
 *	绘制文本
 */
class CSimpleDUIButton : public CSimpleDUIBase
{
public:
	CSimpleDUIButton(CSimpleDUIBase* parent, UINT uID);
	virtual ~CSimpleDUIButton();

public:
	int			m_status;
	UINT		m_uID;

public:
	virtual BOOL		OnEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void		OnDraw(HDC hDC, LPRECT lpRect);
};