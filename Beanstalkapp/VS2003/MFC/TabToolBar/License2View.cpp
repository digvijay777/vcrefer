// ListDataView.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "License2View.h"
#include ".\license2view.h"
#include "LicenseManagerDoc.h"
#include "MainFrm.h"

// CLicense2View

IMPLEMENT_DYNCREATE(CLicense2View, CListView)

CLicense2View::CLicense2View()
{
}

CLicense2View::~CLicense2View()
{
}

BEGIN_MESSAGE_MAP(CLicense2View, CListView)
	ON_WM_CREATE()
	ON_MESSAGE(UM_UPDATESHOW, OnUpdateShow)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_COMMAND(IDM_MODIFY, OnModify)
	ON_UPDATE_COMMAND_UI(IDM_DELETE, OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(IDM_MODIFY, OnUpdateModify)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_COMMAND(IDM_ADDLICENSED, OnAddlicensed)
//	ON_WM_PAINT()
ON_COMMAND(IDM_SCANINFO, OnScaninfo)
END_MESSAGE_MAP()


// CLicense2View diagnostics

#ifdef _DEBUG
void CLicense2View::AssertValid() const
{
	CListView::AssertValid();
}

void CLicense2View::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CLicense2View message handlers

int CLicense2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

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
	list.InsertColumn(1, "�û�ID", LVCFMT_CENTER, 170);
	list.InsertColumn(2, "�û���λ", LVCFMT_LEFT, 150);
	list.InsertColumn(3, "��ϵ�绰", LVCFMT_LEFT, 100);
	list.InsertColumn(4, "�����", LVCFMT_LEFT, 100);
	list.InsertColumn(5, "��Ȩ������", LVCFMT_CENTER, 80);
	list.InsertColumn(6, "����ʱ��", LVCFMT_CENTER, 120);
	list.InsertColumn(7, "ʵ��������", LVCFMT_CENTER, 80);
	list.InsertColumn(8, "��Ȩʱ��", LVCFMT_CENTER, 80);
//	list.InsertColumn(, "�豸���", LVCFMT_CENTER, 100);
	list.InsertColumn(9, "��Ȩ����", LVCFMT_CENTER, 80);
	list.InsertColumn(10, "�������", LVCFMT_CENTER, 60);

	//�������
	PostMessage(UM_UPDATESHOW, 0, 0);

	//���ز˵�
	m_menu.LoadMenu(IDR_CONTEXTMENU);
	return 0;
}

CLicense2Doc* CLicense2View::GetDocument()
{
	return (CLicense2Doc*)(CLicenseManagerDoc *)CListView::GetDocument();
}

LRESULT CLicense2View::OnUpdateShow(WPARAM wParam, LPARAM lParam)
{
	//�������
	CString sql;
	sql = "Select UserSN, CustName, Tel, AttachName, PermitClient, AuthorizationDate, CurrClient, MatureDate, " \
		" case Type when 1 then '��ҵ�û�' else '�����û�' end as szType, ChangeNum from license2 order by szType desc,AuthorizationDate";
	GetDocument()->FillListFromSQL(sql, &GetListCtrl());
	return 0;
}
void CLicense2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListView::OnRButtonDown(nFlags, point);
	// ���˵�
	CMenu* pMenu = m_menu.GetSubMenu(0);
	if(NULL == pMenu)
		return;
	POINT	pt = {point.x, point.y};
	this->ClientToScreen(&pt);
	if(GetListCtrl().GetSelectedCount() < 1)
		UpdateContextMenu(0x1);
	else
		UpdateContextMenu(0x1 | 0x2 | 0x4 | 0x8);
	
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y,  this);
}
void CLicense2View::UpdateContextMenu(DWORD dwMask)
{
	//�����²˵�����ʾ��
	CMenu*	pMenu = m_menu.GetSubMenu(0);
	if(NULL == pMenu)
		return;
	pMenu->EnableMenuItem(IDM_ADDLICENSED, (dwMask & 0x1) != 0x1);
	pMenu->EnableMenuItem(IDM_MODIFY, (dwMask & 0x2) != 0x2);
	pMenu->EnableMenuItem(IDM_DELETE, (dwMask & 0x4) != 0x4);
	pMenu->EnableMenuItem(IDM_SCANINFO, (dwMask & 0x8) != 0x8);
}

void CLicense2View::OnDelete()
{
	// ɾ����
	CListCtrl& list = GetListCtrl();
	int		nSelect		= list.GetNextItem(-1, LVNI_SELECTED);
	if(nSelect < 0)
		return;
	CString		szID;
	szID = list.GetItemText(nSelect, 1);
	GetDocument()->DeleteLicensed(szID.GetBuffer());
}

void CLicense2View::OnModify()
{
	// �޸Ĳ���
	CListCtrl&		list	= GetListCtrl();
	int				nIndex	= list.GetNextItem(-1, LVNI_SELECTED);
	
	if(nIndex < 0)
		return ;
	CHAR	szID[100]		= {0};
	list.GetItemText(nIndex, 1, szID, 100);

	GetDocument()->ModifyLicensed(szID);
}

void CLicense2View::OnUpdateDelete(CCmdUI *pCmdUI)
{
	// ����ɾ���˵�
	pCmdUI->Enable(GetListCtrl().GetNextItem(-1, LVNI_SELECTED) >= 0);
}

void CLicense2View::OnUpdateModify(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(GetListCtrl().GetNextItem(-1, LVNI_SELECTED) >= 0);
}

void CLicense2View::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		if(lplvcd->nmcd.dwItemSpec == GetListCtrl().GetNextItem(-1, LVNI_SELECTED))
		{
			lplvcd->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			lplvcd->clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		*pResult = CDRF_DODEFAULT;
		break;
	default:
		*pResult = CDRF_DODEFAULT;
	}
}


void CLicense2View::OnAddlicensed()
{
	// ���
	GetDocument()->Addlicensed();
}

//void CLicense2View::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CListView::OnPaint() for painting messages
//}

void CLicense2View::OnScaninfo()
{
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
