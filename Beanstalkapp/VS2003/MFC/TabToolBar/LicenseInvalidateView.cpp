// LicenseInvalidateView.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "LicenseInvalidateView.h"

#include "LicenseManagerDoc.h"
#include ".\licenseinvalidateview.h"
#include "MainFrm.h"
// CLicenseInvalidateView

IMPLEMENT_DYNCREATE(CLicenseInvalidateView, CListView)

CLicenseInvalidateView::CLicenseInvalidateView()
{
}

CLicenseInvalidateView::~CLicenseInvalidateView()
{
}

BEGIN_MESSAGE_MAP(CLicenseInvalidateView, CListView)
	ON_WM_CREATE()
	ON_MESSAGE(UM_UPDATESHOW, OnUpdateList)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_SCANINFO, OnScaninfo)
END_MESSAGE_MAP()


// CLicenseInvalidateView diagnostics

#ifdef _DEBUG
void CLicenseInvalidateView::AssertValid() const
{
	CListView::AssertValid();
}

void CLicenseInvalidateView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

CLicenseInvalidateDoc*	CLicenseInvalidateView::GetDocument()
{
	return (CLicenseInvalidateDoc *)(CLicenseManagerDoc *)CListView::GetDocument();
}
// CLicenseInvalidateView message handlers

int CLicenseInvalidateView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CListCtrl&		list = GetListCtrl();
	// �б���ͼ
	DWORD	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD	dwExStyle = list.GetExtendedStyle();

	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_REPORT;
	dwStyle |= LVS_SINGLESEL;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	list.SetExtendedStyle(dwExStyle);

	//�б�ͷ
	list.InsertColumn(0, "���", LVCFMT_CENTER, 40);
	list.InsertColumn(1, "��ȨKEY", LVCFMT_CENTER, 170);
	list.InsertColumn(2, "����ʱ��", LVCFMT_CENTER, 160);

	// ������ͼ
	PostMessage(UM_UPDATESHOW, 0, 0);

	m_menu.LoadMenu(IDR_CONTEXTMENU);
	return 0;
}

// �����б�
LRESULT CLicenseInvalidateView::OnUpdateList(WPARAM wParam, LPARAM lParam)
{
	CString		szSQL;
	CString		szCount;

	szSQL.Format("Select UserSN, LastTime from InvalidLicense2 order by LastTime");
	GetDocument()->FillListFromSQL(szSQL.GetBuffer(), &GetListCtrl());
	return 0;
}
void CLicenseInvalidateView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListView::OnRButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	CMenu* pMenu = m_menu.GetSubMenu(2);
	if(NULL == pMenu)
		return;
	POINT	pt = {point.x, point.y};
	this->ClientToScreen(&pt);
	if(GetListCtrl().GetSelectedCount() < 1)
		UpdateContextMenu(0);
	else
		UpdateContextMenu(0x1);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y,  this);

}
void CLicenseInvalidateView::UpdateContextMenu(DWORD dwMask)
{
	//�����²˵�����ʾ��
	CMenu*	pMenu = m_menu.GetSubMenu(2);
	if(NULL == pMenu)
		return;
	pMenu->EnableMenuItem(IDM_SCANINFO, (dwMask & 0x1) != 0x1);
}
void CLicenseInvalidateView::OnScaninfo()
{
	// TODO: Add your command handler code here
	CListCtrl& list = GetListCtrl();
	int		nSelect	= list.GetNextItem(-1, LVNI_SELECTED);
	if(nSelect < 0)
		return;
	CString		szID;
	szID = list.GetItemText(nSelect, 1);
	szID += "%";
	CMainFrame*		pFrame		= (CMainFrame *)AfxGetMainWnd();
	if(NULL == pFrame)
		return;
	pFrame->m_wndTabCtrl.SetCurSel(1);
	GetDocument()->NotifyView(UM_UPDATESHOW, 1, (LPARAM)szID.GetBuffer());
}
