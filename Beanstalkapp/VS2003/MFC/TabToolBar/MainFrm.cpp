#include "..\splittertest\mainfrm.h"
#include "..\splittertest\mainfrm.h"
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "LicenseManager.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "License2View.h"
#include "LicenseManagerDoc.h"
#include "LicenseManagerView.h"
#include "LicenseInfoView.h"
#include "LicenseInvalidateView.h"
#include ".\mainfrm.h"
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_UPDATESEPARATOR, OnUpdateSeparator)
	ON_COMMAND(IDM_SEARCH, OnSearch)
	ON_UPDATE_COMMAND_UI(IDM_SEARCH, OnUpdateSearch)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	IDS_ALLCOUNT
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
	m_iToolBar.DeleteImageList();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndSearchBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndSearchBar.LoadToolBar(IDR_TOOLSEARCH))
	{
		TRACE0("δ�ܴ�������������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	// ������
	m_wndSearchBar.GetToolBarCtrl().SetButtonSize(CSize(500, 30));
	RECT	rect	= {3, 3, 130, 100};
	m_cbSearch.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST, rect, &m_wndSearchBar, 1);
	rect.bottom = 24;
	rect.left = rect.right + 5;
	rect.right = rect.left + 300;
	m_etSearch.Create(WS_CHILD | WS_VISIBLE | WS_DLGFRAME , rect, &m_wndSearchBar, 2);
	m_etSearch.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
// 	m_etSearch.CreateEx(WS_EX_CLIENTEDGE, "edit", "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rect, &m_wndToolBar, 2);
	rect.left = rect.right + 5;
	rect.right = rect.left + 50;
	m_btSearch.Create("����", WS_CHILD | WS_VISIBLE, rect, &m_wndSearchBar, IDC_TB_SEARCH);
	m_btSearch.SetFont(m_wndToolBar.GetFont());
	m_etSearch.SetFont(m_wndToolBar.GetFont());
	m_cbSearch.SetFont(m_wndToolBar.GetFont());
	int nIndex = m_cbSearch.AddString("�û�KEY");
	m_cbSearch.SetItemData(nIndex, 1);
	nIndex = m_cbSearch.AddString("MAC");
	m_cbSearch.SetItemData(nIndex, 2);
	nIndex = m_cbSearch.AddString("��һ��ʹ��ʱ��");
	m_cbSearch.SetItemData(nIndex, 3);
	nIndex = m_cbSearch.AddString("���һ��ʹ��ʱ��");
	m_cbSearch.SetItemData(nIndex, 4);
	m_cbSearch.SetCurSel(0);

	m_wndSearchBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	RECT rt;
	this->RecalcLayout();     //�ؼ���һ������������
	m_wndToolBar.GetWindowRect(&rect);
	m_wndSearchBar.GetWindowRect(&rt);
	rt.left = rect.right + 10;
	rt.right += rect.right - rect.left;
	rt.top = rect.top;
	rt.bottom = rect.bottom;
	DockControlBar(&m_wndSearchBar, AFX_IDW_DOCKBAR_TOP, &rt);
	//ͼ�깤����
	int icon[]	= {IDI_ADD, IDI_MODIFY, IDI_DELETE};
	int iconG[]	= {IDI_ADDG, IDI_MODIFYG, IDI_DELETEG};
	m_iToolBar.Create(24, 24, ILC_COLOR32 | ILC_MASK, sizeof(icon) / sizeof(int), 0);
	m_iToolBar.SetBkColor(GetSysColor(COLOR_MENUBAR));
	m_iToolBarG.Create(24, 24, ILC_COLOR32 | ILC_MASK, sizeof(iconG) / sizeof(int), 0);
	m_iToolBarG.SetBkColor(GetSysColor(COLOR_MENUBAR));

	for(int i = 0; i < sizeof(icon) / sizeof(int); i++)
	{
		HICON hbm = (HICON)::LoadImage(AfxGetInstanceHandle(),
			MAKEINTRESOURCE(icon[i]),
			IMAGE_ICON,//IMAGE_BITMAP,
			0, 0, 0);

		if(hbm)
			m_iToolBar.Add(hbm);
	}
	for(int i = 0; i < sizeof(iconG) / sizeof(int); i++)
	{
		HICON hbm = (HICON)::LoadImage(AfxGetInstanceHandle(),
			MAKEINTRESOURCE(iconG[i]),
			IMAGE_ICON,//IMAGE_BITMAP,
			0, 0, 0);

		if(hbm)
			m_iToolBarG.Add(hbm);
	}

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_iToolBar);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_iToolBarG);
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(31, 30));
	// TABTOOL
	m_wndTabCtrl.Create(this, IDD_TABTOOLBAR, CBRS_TOP, 1);

	CCreateContext*		pContext;
	CDocument*			pDoc = ((CCreateContext*)lpCreateStruct->lpCreateParams)->m_pCurrentDoc;
	POSITION			ps		= {0};
	ps = pDoc->GetFirstViewPosition();
	m_wndTabCtrl.InsertItem(0, "��Ȩ�����", pDoc->GetNextView(ps));
	m_wndTabCtrl.InsertItem(1, "��Ȩʹ����Ϣ", pDoc, RUNTIME_CLASS(CLicenseInfoView));
	m_wndTabCtrl.InsertItem(2, "������Ȩ��", pDoc, RUNTIME_CLASS(CLicenseInvalidateView));
//  	m_wndTabCtrl.InsertItem(1, "��ͼ3", pDoc, RUNTIME_CLASS(CLicense2View));
// 	m_wndTabCtrl.InsertItem(2, "��ͼ2", pDoc, RUNTIME_CLASS(CEditView));
	SendMessage(UM_UPDATESEPARATOR, 1, (LPARAM)"");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

LRESULT CMainFrame::OnUpdateSeparator(WPARAM wParam, LPARAM lParam)
{
	LPCSTR			lpText	= (LPCSTR)lParam;
	if(NULL == lpText)
		return -1;
	m_wndStatusBar.SetPaneText((int)wParam, lpText);
	return 0;
}
// CMainFrame ��Ϣ�������


void CMainFrame::OnSearch()
{
	m_wndSearchBar.ShowWindow(!m_wndSearchBar.IsWindowVisible());
	RecalcLayout();
	DockControlBar(&m_wndSearchBar);
}

void CMainFrame::OnUpdateSearch(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndSearchBar.IsWindowVisible());
}

