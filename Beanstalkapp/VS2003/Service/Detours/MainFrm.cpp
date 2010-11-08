// MainFrm.cpp : CMainFrame ���ʵ��
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

	// ����TABTOOL������
	m_wndTabBar.Create(this, IDD_TABTOOLBAR, CBRS_TOP, 1);

	CDocument*			pDoc = ((CCreateContext*)lpCreateStruct->lpCreateParams)->m_pCurrentDoc;
	POSITION			ps		= {0};
	ps = pDoc->GetFirstViewPosition();
	m_wndTabBar.InsertItem(0, "��ҳ", pDoc->GetNextView(ps));
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
// ���¹����� wParam Ϊ���¹�������MASKֵ 0x1 �ļ����
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

// lParam : -1 ɾ��, 0 ������, 1 ����/ѡ��
LRESULT CMainFrame::OnUpdateTable(WPARAM wParam, LPARAM lParam)
{
	CDocument* pDoc = GetActiveDocument();
	if(NULL == pDoc)
		return -1;
	CRuntimeClass*		pRuntimClass[]	= { RUNTIME_CLASS(CDetFileView), 
											RUNTIME_CLASS(CDetRegView)};
	CHAR*				szExplain[]		= { "�ļ����",					
											"ע�����"};
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
	// TODO: �ڴ���������������
	UINT  uCheck = GetMenu()->GetSubMenu(1)->GetMenuState(IDM_DET_FILE, MF_BYCOMMAND);
	if(COMPAREINCLUDE(uCheck, MF_CHECKED))
		SendMessage(UM_FRAME_UPDATETABLE, 0x1, -1);
	else
		SendMessage(UM_FRAME_UPDATETABLE, 0x1, 1);
}

void CMainFrame::OnUpdateDetFile(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(FindViewIntable(RUNTIME_CLASS(CDetFileView)) > 0);
}

void CMainFrame::OnDetReg()
{
	// TODO: �ڴ���������������
	UINT  uCheck = GetMenu()->GetSubMenu(1)->GetMenuState(IDM_DET_REG, MF_BYCOMMAND);
	if(COMPAREINCLUDE(uCheck, MF_CHECKED))
		SendMessage(UM_FRAME_UPDATETABLE, 0x2, -1);
	else
		SendMessage(UM_FRAME_UPDATETABLE, 0x2, 1);
}

void CMainFrame::OnUpdateDetReg(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(FindViewIntable(RUNTIME_CLASS(CDetRegView)) > 0);
}
