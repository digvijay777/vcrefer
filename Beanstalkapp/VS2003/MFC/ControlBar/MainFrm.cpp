// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ControlBar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	RECT rect = {0, 0, 200, 100} ;
//	m_wndTabCtrl.CreateEx(WS_EX_CLIENTEDGE, "SysTabControl32", "", WS_VISIBLE | WS_CHILD, rect, this, 1);
//	m_wndTabCtrl.Create("#32770", "", WS_VISIBLE | WS_CHILD, rectDefault, this, 0);
	if(!m_wndDlgBar.Create(this, IDD_BAR_DLGBAR, WS_VISIBLE | WS_CHILD, IDD_BAR_DLGBAR))
	{
		TRACE0("创建DialogBar失败");
		return -1;
	}
	m_wndDlgBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
    RecalcLayout();
	m_wndToolBar.GetWindowRect(&rect);
	rect.right = 2 * rect.right - rect.left;
	rect.left = rect.right;
	RECT rectDlg;
	m_wndDlgBar.GetWindowRect(&rectDlg);
	rectDlg.bottom = rectDlg.top + rect.bottom - rect.left;
	m_wndDlgBar.MoveWindow(&rectDlg);
	DockControlBar(&m_wndDlgBar, AFX_IDW_DOCKBAR_TOP, &rect);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序

