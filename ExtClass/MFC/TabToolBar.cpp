// TabToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "TabToolBar.h"

// #include "../Common/NSimple.h"
// CTabToolBar

IMPLEMENT_DYNAMIC(CTabToolBar, CDialogBar)
CTabToolBar::CTabToolBar()
{
}

CTabToolBar::~CTabToolBar()
{
}


BEGIN_MESSAGE_MAP(CTabToolBar, CDialogBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnTabCtrlSelectChange)
END_MESSAGE_MAP()



// CTabToolBar message handlers


int CTabToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建TAB控件
	RECT	rect	= {0};
	m_wndTabCtrl.Create(WS_VISIBLE | WS_CHILD | TBS_TOP, rect, this, IDC_TABCTRL);
	static CFont  font;
	font.CreatePointFont(90, "宋体");
	m_wndTabCtrl.SetFont(&font);
	m_wndTabCtrl.SetItemSize(CSize(0, 20));
	GetWindowRect(&rect);
	rect.bottom = rect.top + 26;
	MoveWindow(&rect);
	return 0;
}

void CTabToolBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// 改变TAB控件大小
	RECT	rect	= {0};
	GetClientRect(&rect);
	rect.bottom += 2;
	rect.right += 1;
	rect.left -= 0;
	rect.top += 2;
	m_wndTabCtrl.MoveWindow(&rect);
}

// 添加新项 新建视图
BOOL CTabToolBar::InsertItem(int nIndex, LPSTR lpTitle, CCreateContext* pContext)
{
	TCITEM		item	= {0};
	CView*		pView	= NULL;
	int			nRet	= 0;
	CFrameWnd*	pFrame	= NULL;
	CView*		pOldView= NULL;
	static		sID		= AFX_IDW_PANE_FIRST + 1;
		
	item.mask = TCIF_PARAM | TCIF_TEXT;
	item.pszText = lpTitle;
	pFrame = (CFrameWnd *)GetParent();
	if(NULL == pFrame)
		return FALSE;
	pView = (CView *)pContext->m_pNewViewClass->CreateObject();
	if(NULL == pView)
		return FALSE;
	pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CFrameWnd::rectDefault, pFrame, sID++, NULL);
	if(!IsWindow(pView->m_hWnd))
		return false;
	pContext->m_pCurrentDoc->AddView(pView);
	item.lParam = (LPARAM)pView;

	nRet = m_wndTabCtrl.InsertItem(nIndex, &item);
	return nRet > -1;
}
// 添加新项 新建视图
BOOL CTabToolBar::InsertItem(int nIndex, LPSTR lpTitle, CDocument* pDoc, CRuntimeClass* pView)
{
	CCreateContext			context;
 	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = pView;
	return InsertItem(nIndex, lpTitle, &context);
}
// 添加新项　原用视图
BOOL CTabToolBar::InsertItem(int nIndex, LPSTR lpTitle, CView* pView)
{
	TCITEM		item	= {0};
	int			nRet	= 0;

	item.mask = TCIF_PARAM | TCIF_TEXT;
	item.pszText = lpTitle;
	item.lParam = (LPARAM)pView;

	nRet = m_wndTabCtrl.InsertItem(nIndex, &item);
	return nRet > -1;
}
// TAB改变
void CTabToolBar::OnTabCtrlSelectChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	// 更改视图
	CView*		pOldView	= NULL;
	CView*		pNewView	= NULL;
	TCITEM		item		= {0};
	int			nSelItem	= 0;
	CFrameWnd*	pFrame		= (CFrameWnd *)GetParent();
	RECT		rect		= {0};
	CDocument*	pDoc		= NULL;

    if(NULL == pFrame)
		return;
	pDoc = pFrame->GetActiveDocument();
	if(NULL == pDoc)
		return;
	pOldView = pFrame->GetActiveView();
	if(NULL == pOldView)
		return;
	nSelItem = m_wndTabCtrl.GetCurSel();
	item.mask = TCIF_PARAM;
	m_wndTabCtrl.GetItem(nSelItem, &item);
	pNewView = (CView *)item.lParam;
	if(NULL == pNewView)
		return;
	// 更换视图
	int nSwitchChildID = pNewView->GetDlgCtrlID();
	pNewView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	pOldView->SetDlgCtrlID(nSwitchChildID);
	pNewView->ShowWindow(SW_SHOW);
	pOldView->ShowWindow(SW_HIDE);
	pFrame->SetActiveView(pNewView);
	pFrame->RecalcLayout();
}

int CTabToolBar::SetCurSel(int nItem)
{
	m_wndTabCtrl.SetCurSel(nItem);
	LRESULT n = 0;
	OnTabCtrlSelectChange(NULL, &n);
	return 0;
}

CTabCtrl& CTabToolBar::GetTabCtrl()
{
	return m_wndTabCtrl;
}

BOOL CTabToolBar::DeleteItem(int nIndex)
{
	if(m_wndTabCtrl.GetItemCount() < 1)
		return FALSE;

	TCITEM		item	= {0};

	item.mask = TCIF_PARAM;
	m_wndTabCtrl.GetItem(nIndex, &item);

	CView*		pView = (CView *)item.lParam;
	if(NULL != pView)
	{
		CDocument*	pDoc = pView->GetDocument();
		if(nIndex == m_wndTabCtrl.GetItemCount() - 1)
			SetCurSel(nIndex - 1);
		else
			SetCurSel(nIndex + 1);
		pDoc->RemoveView(pView);
		pView->DestroyWindow();
	}
	return m_wndTabCtrl.DeleteItem(nIndex);
}