// ToolView.cpp : implementation file
//

#include "stdafx.h"
#include "ClientMgr.h"
#include "ToolView.h"
#include ".\toolview.h"


// CToolView

IMPLEMENT_DYNAMIC(CToolView, CReBar)
CToolView::CToolView()
{
}

CToolView::~CToolView()
{
}


BEGIN_MESSAGE_MAP(CToolView, CReBar)
END_MESSAGE_MAP()

// CToolView message handlers
BOOL CToolView::Create(CWnd* pParentWnd, CRuntimeClass* pView, CSize size, UINT nID)
{
	DWORD		dwStyle		= 0;
	BOOL		bSuccess	= FALSE;
	DWORD		dwStyleCtrl = 0;

	dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM;
	dwStyleCtrl = RBS_AUTOSIZE | CCS_BOTTOM | CCS_NODIVIDER | RBS_BANDBORDERS;
	bSuccess = CReBar::Create(pParentWnd, dwStyleCtrl, dwStyle, nID);
	if(!bSuccess)
		return FALSE;

	// 添加视图
	CView*			pWnd		= (CView *)pView->CreateObject();
	CReBarCtrl&		reBar		= GetReBarCtrl();
	REBARBANDINFO	info		= {0};

	if(NULL != pWnd)
		pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, size.cx, size.cy), this, 1);
    CReBar::AddBar(pWnd);
	// 改变属性
	info.cbSize = sizeof(REBARBANDINFO);
	info.fMask = RBBIM_STYLE;
	info.fStyle = RBBS_BREAK | RBBS_NOGRIPPER | RBBS_FIXEDSIZE;
	reBar.SetBandInfo(0, &info);

	return bSuccess;
}

