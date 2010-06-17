// MenuBar.cpp : implementation file
//

#include "stdafx.h"
#include "SmallSkin.h"
#include "MenuBar.h"


// CMenuBar

IMPLEMENT_DYNAMIC(CMenuBar, CToolBar)
CMenuBar::CMenuBar()
{
	m_hMenu		= NULL;
}

CMenuBar::~CMenuBar()
{
}


BEGIN_MESSAGE_MAP(CMenuBar, CToolBar)
END_MESSAGE_MAP()

BOOL CMenuBar::SetMenu(HWND hWnd, HMENU hMenu)
{
	SetParent(hWnd);
	m_hMenu = hMenu;
	if(NULL == GetSafeHwnd() && FALSE == CreateMenuBar())
		return FALSE;

	return UpdateMenu();
}

// CMenuBar message handlers

