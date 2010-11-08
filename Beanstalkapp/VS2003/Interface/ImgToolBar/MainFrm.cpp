// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ImgToolBar.h"
// #include <atlimage.h>
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

	//////////////////////////////////////////////////////////////////////////
	// 美化工具栏
	UINT	ico[]	= {IDI_NEW, IDI_OPEN, IDI_SAVE, IDI_SET, IDI_REFURBISH, IDI_PRINT, IDI_HELP};
	UINT	icoG[]	= {IDI_NEWG, IDI_OPENG, IDI_SAVEG, IDI_SETG, IDI_REFURBISHG, IDI_PRINTG, IDI_HELPG};
	
	m_imgToolBar.Create(32, 32, ILC_COLOR32 | ILC_MASK, 7, 1);
	m_imgToolBar.SetBkColor(GetSysColor(COLOR_MENUBAR));
	m_imgToolBarG.Create(32, 32, ILC_COLOR | ILC_MASK, 7, 1);
	m_imgToolBarG.SetBkColor(GetSysColor(COLOR_MENUBAR));
	for(int i = 0; i < sizeof(ico) / sizeof(DWORD); i++)
	{
//		CImage		img;
//		img.Load("X:\\VS2003\\Interface\\ImgToolBar\\res\\20080512090208714.png");
//		img.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(png[i]));
		//m_imgToolBar.
		HICON	hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(ico[i]));
 		m_imgToolBar.Add(hIcon);
		//DeleteObject(hIcon);
		//HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(ico[i]));
		m_imgToolBarG.Add(hIcon);
		DeleteObject(hIcon);
	}
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imgToolBar);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_imgToolBarG);
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(40, 50));
	// 禁用项
	m_wndToolBar.SetButtonInfo(1, 0x9000, TBBS_BUTTON, 1);
	// 添加提示项
	m_wndToolBar.SetButtonText(0, "New");
	m_wndToolBar.SetButtonText(1, "Open");
	// 添加新项
	
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

