// LogToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "LogToolBar.h"
#include ".\logtoolbar.h"
#include <afxpriv.h>
/*#include <../src/mfc/afximpl.h>*/
// CDialogBar
// CLogToolBar

IMPLEMENT_DYNAMIC(CLogToolBar, CDialogBar)
CLogToolBar::CLogToolBar()
{
}

CLogToolBar::~CLogToolBar()
{
}


BEGIN_MESSAGE_MAP(CLogToolBar, CDialogBar)
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()



// CLogToolBar message handlers


BOOL CLogToolBar::Create(CWnd* pParentWnd, UINT nStyle, CSize size, UINT nID)
{
	static DLGTEMPLATE	dlgTmp	= {WS_VISIBLE|WS_CHILD};

// 	ASSERT(pParentWnd != NULL);
// 	CControlBar::Create()
// 	bRes = CDialogBar::CreateDlgIndirect(&dlgTmp, pParentWnd, AfxGetInstanceHandle());
// 	if(!bRes)
// 	{
// 		// create failed
// 		return FALSE;
// 	}
	ASSERT(pParentWnd != NULL);

	// allow chance to modify styles
	m_dwStyle = (nStyle & CBRS_ALL);

	BOOL bSuccess = CreateDlgIndirect(&dlgTmp, pParentWnd, AfxGetInstanceHandle());
	if (!bSuccess)
		return FALSE;

	// dialog template MUST specify that the dialog
	//  is an invisible child window
	SetDlgCtrlID(nID);
	m_sizeDefault = size;    // set fixed size

	// force WS_CLIPSIBLINGS
	ModifyStyle(0, WS_CLIPSIBLINGS);

	if (!ExecuteDlgInit(&dlgTmp))
		return FALSE;

	// force the size to zero - resizing bar will occur later
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);

	// set size
	SetFont(pParentWnd->GetFont());

	return TRUE;
}


void CLogToolBar::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogBar::OnWindowPosChanged(lpwndpos);

	if(IsFloating()) // ¸¡¶¯²Ù×÷
	{

	}

	if(m_pDockBar)
	{
// 		CWnd*	pParent		= CControlBar::m_pDockBar->GetParent();
// 
// 		if(pParent)
// 		{
// 			CRect		rect;
// 
// 			pParent->GetClientRect(&rect);
// 			m_sizeDefault = rect.Size();
// 			SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOACTIVATE|SWP_SHOWWINDOW);
// 		}
	}
}
