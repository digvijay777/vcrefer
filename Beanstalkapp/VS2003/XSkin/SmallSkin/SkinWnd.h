#pragma once

#define STR_SKINWND_PROP			"SmallSkin"
#define STR_SKINWND_PROP_PTR		"SmallSkinPtr"

class CSkinWnd
{
public:
	CSkinWnd(HWND hWnd);
	virtual ~CSkinWnd(void);

public:
	HWND		m_hWnd;
public:
	virtual BOOL	WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lRet);
	virtual BOOL	Destroy();
	virtual BOOL	OnNcActive();
	virtual BOOL	OnNcPrint();
	virtual BOOL	OnEraseBkGng(CDC* pDC);
};
