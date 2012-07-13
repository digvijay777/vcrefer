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
} DUI_ROOT, *PDUI_ROOT;
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
	BOOL				m_isVisible;

protected:
	static void	DeleteDUI(CSimpleDUIBase* pUI, BOOL bAutoDel = TRUE);

public:
	CSimpleDUIBase*		GetChildUI();
	CSimpleDUIBase*		GetBrotherUI();

public:
	BOOL	DispatchUIEvent(POINT pt, UINT nMsg, 
		WPARAM wParam, LPARAM lParam);
	void	TranslateTrackEvent();
	void	MoveUI(LPRECT lpRect, BOOL bInvalidate = TRUE);
	void	DrawUI(HDC hDC, LPRECT lpRect);
	BOOL	IsVisible();
	void	ShowUI(BOOL bShow);
	BOOL	SetUICapture();
	BOOL	ReleaseUICapture();
	void	SetUIFocus();
	void	KillUIFocus();
	void	TrackEvent(UINT nMsg);
	void	RectToPanel(RECT* rect);
	void	GetUIRect(RECT* rect);
	void	ForegroundUI();

public:
	virtual void				UIInvalidate(LPRECT lpRect = NULL);
	virtual PDUI_ROOT			GetUIRoot();
	virtual BOOL				OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void				OnUIDraw(HDC hDC, LPRECT lpRect) = 0;

public:
	static void						MergerRect(RECT* des, LPRECT rt1, LPRECT rt2);
};

/*
 *	主面板
 */
class CSimpleDUIRoot : protected CSimpleDUIBase
{
public:
	CSimpleDUIRoot();
	virtual ~CSimpleDUIRoot();

private:
	DUI_ROOT					m_root;
	BOOL						m_bTrackEvent;

public:
	BOOL			TranslateUIEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	void			PaintUI(HWND hWnd, HDC hDC);

public:
	virtual PDUI_ROOT			GetUIRoot();
	virtual void				EraseUIBkgnd(HDC hDC, HWND hWnd);
	virtual void				UIInvalidate(LPRECT lpRect);
	virtual void				OnUIDraw(HDC hDC, LPRECT lpRect);
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
	virtual		void	OnUIDraw(HDC hDC, LPRECT lpRect);

public:
	void	SetText(LPCWSTR lpText);
};

/*
 *	绘制按钮样本
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
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
};

/*
 *	绘制面板
 */
class CSimpleDUIPanel : public CSimpleDUIBase
{
public:
	CSimpleDUIPanel(CSimpleDUIBase* parent, COLORREF col, BYTE alpha = 0xff);
	virtual ~CSimpleDUIPanel();

public:
	COLORREF		m_color;
	BYTE			m_alpha;

public:
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
};

