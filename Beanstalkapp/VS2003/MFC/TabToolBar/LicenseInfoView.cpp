// LicenseInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "LicenseInfoView.h"
#include ".\licenseinfoview.h"
#include "LicenseManagerDoc.h"
#include "MainFrm.h"

// CLicenseInfoView

IMPLEMENT_DYNCREATE(CLicenseInfoView, CListView)

CLicenseInfoView::CLicenseInfoView()
{
}

CLicenseInfoView::~CLicenseInfoView()
{
}

BEGIN_MESSAGE_MAP(CLicenseInfoView, CListView)
	ON_WM_CREATE()
	ON_MESSAGE(UM_UPDATESHOW, OnUpdateList)
	ON_COMMAND(IDC_TB_SEARCH, OnTbSearch)
	ON_COMMAND(IDM_MODIFY, OnModify)
	ON_UPDATE_COMMAND_UI(IDM_MODIFY, OnUpdateModify)
END_MESSAGE_MAP()


// CLicenseInfoView diagnostics

#ifdef _DEBUG
void CLicenseInfoView::AssertValid() const
{
	CListView::AssertValid();
}

void CLicenseInfoView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CLicenseInfoView message handlers

int CLicenseInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CListCtrl&		list = GetListCtrl();
	// 列表视图
	DWORD	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD	dwExStyle = list.GetExtendedStyle();

	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_REPORT;
// 	dwStyle |= LVS_SINGLESEL;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	list.SetExtendedStyle(dwExStyle);

	//列表头
	list.InsertColumn(0, "序号", LVCFMT_CENTER, 40);
	list.InsertColumn(1, "授权KEY", LVCFMT_CENTER, 170);
	list.InsertColumn(2, "用户MAC", LVCFMT_CENTER, 160);
	list.InsertColumn(3, "第一次使用时间", LVCFMT_CENTER, 160);
	list.InsertColumn(4, "最近一次使用时间", LVCFMT_CENTER, 160);
	list.InsertColumn(5, "使用次数", LVCFMT_CENTER, 80);

	// 更新视图
	PostMessage(UM_UPDATESHOW, 0, 0);
	return 0;
}

CLicenseInfoDoc* CLicenseInfoView::GetDocument()
{
	return (CLicenseInfoDoc *)(CLicenseManagerDoc *)CListView::GetDocument();
}

//　更新视图
LRESULT CLicenseInfoView::OnUpdateList(WPARAM wParam, LPARAM lParam)
{
	static CString	sOldSQL;	
	LPCSTR		lpCase = (LPCSTR)lParam;
	CString		szSQL;
	if(NULL != lpCase)
	{
		switch(wParam)
		{
		case 1:
			szSQL.Format("Select UserSN,MAC,FristUse,LastUse,UseCount from License2Info Where UserSN like '%s' order by LastUse desc", 
				GetDocument()->TranslateStrForSQL(CString(lpCase)));
			break;
		case 2:
			szSQL.Format("Select UserSN,MAC,FristUse,LastUse,UseCount from License2Info Where MAC like '%s' order by LastUse desc", 
				GetDocument()->TranslateStrForSQL(CString(lpCase)));
			break;
		case 3:
			szSQL.Format("Select UserSN,MAC,FristUse,LastUse,UseCount from License2Info Where FristUse like '%s' order by LastUse desc", 
				GetDocument()->TranslateStrForSQL(CString(lpCase)));
			break;
		case 4:
			szSQL.Format("Select UserSN,MAC,FristUse,LastUse,UseCount from License2Info Where LastUse like '%s' order by LastUse desc", 
				GetDocument()->TranslateStrForSQL(CString(lpCase)));
			break;
		default:
			return -1;
		}
	}
	else if(200 == wParam)
		szSQL = sOldSQL;
	else
		szSQL.Format("Select UserSN,MAC,FristUse,LastUse,UseCount from License2Info order by LastUse desc, UserSN");
	sOldSQL = szSQL;
	GetDocument()->FillListFromSQL(szSQL.GetBuffer(), &GetListCtrl());
	return 0;
}
void CLicenseInfoView::OnTbSearch()
{
	CMainFrame*		pFrame	= (CMainFrame *)AfxGetMainWnd();
	if(NULL == pFrame)
		return;

	CString		szText;
	int			nType		= 0;
	pFrame->m_etSearch.GetWindowText(szText);
	if(szText.GetLength() < 1)
	{
		SendMessage(UM_UPDATESHOW, 0, 0);
		return;
	}
	nType = pFrame->m_cbSearch.GetItemData(pFrame->m_cbSearch.GetCurSel());
	if(szText.Find('%') < 0)
		szText = "%" + szText + "%";
	SendMessage(UM_UPDATESHOW, nType, (WPARAM)szText.GetBuffer());
}

void CLicenseInfoView::OnModify()
{
	// 修改操作
	CListCtrl&		list	= GetListCtrl();
	int				nIndex	= list.GetNextItem(-1, LVNI_SELECTED);

	if(nIndex < 0)
		return ;
	CHAR	szID[100]		= {0};
	list.GetItemText(nIndex, 1, szID, 100);

	GetDocument()->ModifyLicensed(szID);
}

void CLicenseInfoView::OnUpdateModify(CCmdUI *pCmdUI)
{
		pCmdUI->Enable(GetListCtrl().GetNextItem(-1, LVNI_SELECTED) >= 0);
}
