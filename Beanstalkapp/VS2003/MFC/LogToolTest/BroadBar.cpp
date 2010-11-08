// BaseBar.cpp : implementation file
//

#include "stdafx.h"
#include "BroadBar.h"

// CBaseBar
IMPLEMENT_DYNAMIC(CBroadBar, CControlBar)
CBroadBar::CBroadBar()
{
}

CBroadBar::~CBroadBar()
{
}


BEGIN_MESSAGE_MAP(CBroadBar, CControlBar)
END_MESSAGE_MAP()



// CBaseBar message handlers

void CBroadBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}

BOOL CBroadBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= WS_CHILD | WS_VISIBLE  |WS_CLIPSIBLINGS | WS_CLIPCHILDREN \
		| WS_BORDER | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY;
	dwStyle |= RBS_VARHEIGHT | RBS_REGISTERDROP | RBS_AUTOSIZE | RBS_VERTICALGRIPPER \
		| RBS_DBLCLKTOGGLE | TBSTYLE_FLAT;

	return CControlBar::Create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID, NULL);
}
