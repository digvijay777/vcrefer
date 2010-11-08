#pragma once


// CMenuBar

class CMenuBar : public CToolBar
{
	DECLARE_DYNAMIC(CMenuBar)

public:
	CMenuBar();
	virtual ~CMenuBar();

	BOOL	SetMenu(HWND hWnd, HMENU hMenu);
	HMENU	m_hMenu;

	BOOL	CreateMenuBar(HWND hParent);
protected:
	DECLARE_MESSAGE_MAP()
};


