// LogBar.cpp : implementation file
//

#include "stdafx.h"
#include "LogToolTest.h"
#include "LogBar.h"
#include ".\logbar.h"


// CLogBar

IMPLEMENT_DYNAMIC(CLogBar, CReBar)
CLogBar::CLogBar()
{
}

CLogBar::~CLogBar()
{
}


BEGIN_MESSAGE_MAP(CLogBar, CReBar)
END_MESSAGE_MAP()



// CLogBar message handlers


BOOL CLogBar::Create(CWnd* pParentWnd, const RECT& rect, UINT nID)
{

// 	m_ctlBar.Create(TOOLBARCLASSNAME, NULL, CBRS_BOTTOM | WS_VISIBLE | WS_BORDER | WS_CHILD, rect, pParentWnd, nID, NULL);
// 
// 	return TRUE;
	DWORD		dwStyle		= 0;
	BOOL		bSuccess	= FALSE;
	DWORD		dwStyleCtrl = 0;
	
// 	dwStyle = WS_CHILD | WS_VISIBLE  |WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_BORDER;
// 	dwStyle = dwStyle | RBS_AUTOSIZE | RBS_BANDBORDERS | RBS_DBLCLKTOGGLE | RBS_REGISTERDROP | RBS_VARHEIGHT | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_TOP;
// 	bSuccess = CReBar::Create(dwStyle, rect, pParentWnd, nID);
	dwStyle = WS_CHILD | WS_VISIBLE  |WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM \
		| WS_BORDER | CBRS_SIZE_DYNAMIC | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY;
	dwStyleCtrl = RBS_VARHEIGHT | RBS_REGISTERDROP | RBS_AUTOSIZE | RBS_VERTICALGRIPPER \
		| RBS_DBLCLKTOGGLE | CCS_BOTTOM | TBSTYLE_FLAT;
	bSuccess = CReBar::Create(pParentWnd
		, dwStyleCtrl
		, dwStyle
		, nID);
	
	
// 	m_dlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR);
/*	CReBar::AddBar(&m_dlgBar);*/
	// ÃÌº”œÓ
// 	REBARBANDINFO		info1		= {0};
// 	CReBarCtrl&			ctrl		= CReBar::GetReBarCtrl();
// 
// 	info1.cbSize = sizeof(info1);
// 	info1.fMask = RBBIM_CHILD;
// 	info1.hwndChild = CreateWindow(STATUSCLASSNAME, "hello rebar", WS_CHILD | WS_VISIBLE, 0, 0, 100, 30, m_hWnd, NULL, NULL, NULL);
// 	CReBar::MoveWindow(&rect);
// 	ctrl.InsertBand(-1, &info1);
	m_ctlText.Create("hello rebar", WS_VISIBLE | WS_CHILD, rect, this);
	CReBar::AddBar(&m_ctlText);

	return bSuccess;
}

void CLogBar::EnableDocking(DWORD dwDockStyle)
{
	CControlBar::EnableDocking(dwDockStyle);
}