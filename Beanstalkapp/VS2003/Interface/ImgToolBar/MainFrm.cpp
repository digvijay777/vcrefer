// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���������\n");
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

	//////////////////////////////////////////////////////////////////////////
	// ����������
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
	// ������
	m_wndToolBar.SetButtonInfo(1, 0x9000, TBBS_BUTTON, 1);
	// �����ʾ��
	m_wndToolBar.SetButtonText(0, "New");
	m_wndToolBar.SetButtonText(1, "Open");
	// �������
	
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


// CMainFrame ��Ϣ�������

