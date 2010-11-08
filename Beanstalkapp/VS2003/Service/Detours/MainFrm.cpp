// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Detours.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DetFileView.h"
#include "DetRegView.h"
#include ".\mainfrm.h"
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_FRAME_UPDATETABLE, OnUpdateTable)
	ON_COMMAND(IDM_DET_FILE, OnDetFile)
	ON_UPDATE_COMMAND_UI(IDM_DET_FILE, OnUpdateDetFile)
	ON_COMMAND(IDM_DET_REG, OnDetReg)
	ON_UPDATE_COMMAND_UI(IDM_DET_REG, OnUpdateDetReg)
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

	// 创建TABTOOL工具栏
	m_wndTabBar.Create(this, IDD_TABTOOLBAR, CBRS_TOP, 1);

	CDocument*			pDoc = ((CCreateContext*)lpCreateStruct->lpCreateParams)->m_pCurrentDoc;
	POSITION			ps		= {0};
	ps = pDoc->GetFirstViewPosition();
	m_wndTabBar.InsertItem(0, "首页", pDoc->GetNextView(ps));
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
// 更新工具栏 wParam 为更新工具栏的MASK值 0x1 文件监控
int CMainFrame::FindViewIntable(CRuntimeClass* pViewClass)
{
	int		index = 0;
	for(index = m_wndTabBar.GetTabCtrl().GetItemCount() - 1; index >= 0; index--)
	{
		TCITEM		item	= {0};

		item.mask = TCIF_PARAM;
		m_wndTabBar.GetTabCtrl().GetItem(index, &item);
		
		CView*		pView = (CView *)item.lParam;
		if(NULL != pView)
		{
			if(pView->GetRuntimeClass() == pViewClass)
				break;
		}
	}

	return index;
}

// lParam : -1 删除, 0 不操作, 1 新增/选择
LRESULT CMainFrame::OnUpdateTable(WPARAM wParam, LPARAM lParam)
{
	CDocument* pDoc = GetActiveDocument();
	if(NULL == pDoc)
		return -1;
	CRuntimeClass*		pRuntimClass[]	= { RUNTIME_CLASS(CDetFileView), 
											RUNTIME_CLASS(CDetRegView)};
	CHAR*				szExplain[]		= { "文件监控",					
											"注册表监控"};
	int					nMask[]			= { 0x1,
											0x2};

	for(int i = 0; i < sizeof(nMask) / sizeof(int); i++)
	{
		if(COMPAREINCLUDE(wParam, nMask[i]))
		{
			int index = FindViewIntable(pRuntimClass[i]);
			if(index >= 0)
			{
				if(1 == lParam)
					m_wndTabBar.SetCurSel(index);
				else if(-1 == lParam)
					m_wndTabBar.DeleteItem(index);
			}
			else
			{
				if(1 == lParam)
				{
					m_wndTabBar.InsertItem(1, szExplain[i], pDoc, pRuntimClass[i]);
					m_wndTabBar.SetCurSel(1);
				}
			}
		}
	}
	return 0;
}

void CMainFrame::OnDetFile()
{
	// TODO: 在此添加命令处理程序代码
	UINT  uCheck = GetMenu()->GetSubMenu(1)->GetMenuState(IDM_DET_FILE, MF_BYCOMMAND);
	if(COMPAREINCLUDE(uCheck, MF_CHECKED))
		SendMessage(UM_FRAME_UPDATETABLE, 0x1, -1);
	else
		SendMessage(UM_FRAME_UPDATETABLE, 0x1, 1);
}

void CMainFrame::OnUpdateDetFile(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(FindViewIntable(RUNTIME_CLASS(CDetFileView)) > 0);
}

void CMainFrame::OnDetReg()
{
	// TODO: 在此添加命令处理程序代码
	UINT  uCheck = GetMenu()->GetSubMenu(1)->GetMenuState(IDM_DET_REG, MF_BYCOMMAND);
	if(COMPAREINCLUDE(uCheck, MF_CHECKED))
		SendMessage(UM_FRAME_UPDATETABLE, 0x2, -1);
	else
		SendMessage(UM_FRAME_UPDATETABLE, 0x2, 1);
}

void CMainFrame::OnUpdateDetReg(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(FindViewIntable(RUNTIME_CLASS(CDetRegView)) > 0);
}
