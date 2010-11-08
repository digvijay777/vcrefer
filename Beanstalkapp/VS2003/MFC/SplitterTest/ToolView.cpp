// ToolView.cpp : implementation file
//

#include "stdafx.h"
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
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CRect		rect;

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		GetWindowRect(hwndDlg, &rect);
		rect.bottom += 10;
		MoveWindow(hwndDlg, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		break;
	}
	return 0;
}
// CToolView message handlers
BOOL CToolView::Create(CWnd* pParentWnd, CRuntimeClass* pView, CSize size, UINT nID)
{
	DWORD		dwStyle		= 0;
	BOOL		bSuccess	= FALSE;
	DWORD		dwStyleCtrl = 0;

	dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM | WS_BORDER | WS_SIZEBOX;
	dwStyleCtrl = /*RBS_AUTOSIZE |*/ CCS_BOTTOM | CCS_NODIVIDER | RBS_BANDBORDERS;
	bSuccess = CReBar::Create(pParentWnd, dwStyleCtrl, dwStyle, nID);
	if(!bSuccess)
		return FALSE;

	// 创建分隔条
	static DLGTEMPLATE	dlg			= {0};
	HWND				hDlg		= NULL;
	CReBarCtrl&			reBar		= GetReBarCtrl();
	REBARBANDINFO		info		= {0};
	CWnd*				pDlg		= NULL;
	CView*				pWnd		= (CView *)pView->CreateObject();

	dlg.style = WS_CHILD | WS_VISIBLE;
	dlg.cy = 40;
	hDlg = CreateDialogIndirect((HINSTANCE)GetWindowLong(GetSafeHwnd(), GWL_HINSTANCE)
		, &dlg, GetSafeHwnd(), DialogProc);
	pDlg = CWnd::FromHandle(hDlg);
	// 创建视图
	if(NULL != pWnd)
		pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, size.cy), this, 1);

//	CReBar::AddBar(pWnd);
	CReBar::AddBar(pDlg);

// 	for(int i = 0; i < reBar.GetRowCount(); i++)
// 	{
		memset(&info, 0, sizeof(info));
		info.cbSize = sizeof(REBARBANDINFO);
		info.fMask = RBBIM_STYLE | RBBIM_SIZE;
// 		info.hwndChild = hDlg;
		info.fStyle = RBBS_BREAK | RBBS_NOGRIPPER | RBBS_FIXEDSIZE;
		info.cx = size.cy + 4;
		reBar.SetBandInfo(0, &info);
// 		info.hwndChild = pWnd->GetSafeHwnd();
// 		reBar.InsertBand(1, &info);
// 	}

	return bSuccess;
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	int		ht		= SendMessage(WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y));

	TRACE("%08X OnMouseMove point(%d,%d) \tht(%d)\r\n", GetTickCount(), point.x, point.y, ht);
	CReBar::OnMouseMove(nFlags, point);
}
