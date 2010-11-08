// ControlBarView.cpp : CControlBarView 类的实现
//

#include "stdafx.h"
#include "ControlBar.h"

#include "ControlBarDoc.h"
#include "ControlBarView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlBarView

IMPLEMENT_DYNCREATE(CControlBarView, CView)

BEGIN_MESSAGE_MAP(CControlBarView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(IDC_BT_TEST1, OnClickTest1)
	ON_UPDATE_COMMAND_UI(IDC_BT_TEST1, OnUpdateTest1)
END_MESSAGE_MAP()

// CControlBarView 构造/析构

CControlBarView::CControlBarView()
{
	// TODO: 在此处添加构造代码

}

CControlBarView::~CControlBarView()
{
}

BOOL CControlBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CControlBarView 绘制

void CControlBarView::OnDraw(CDC* /*pDC*/)
{
	CControlBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CControlBarView 打印

BOOL CControlBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CControlBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CControlBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CControlBarView 诊断

#ifdef _DEBUG
void CControlBarView::AssertValid() const
{
	CView::AssertValid();
}

void CControlBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CControlBarDoc* CControlBarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CControlBarDoc)));
	return (CControlBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CControlBarView 消息处理程序
void CControlBarView::OnClickTest1()
{
	CMainFrame*		pFrame = (CMainFrame *)GetParent();
	if(NULL == pFrame)
		return;

	CString		szText;
	pFrame->m_wndDlgBar.GetDlgItemText(IDC_EDIT1, szText);
	AfxMessageBox(szText);
}

void CControlBarView::OnUpdateTest1(CCmdUI *pCmdUI)
{
	CMainFrame*		pFrame = (CMainFrame *)GetParent();
	if(NULL == pFrame)
		return;

	CString		szText;
	pFrame->m_wndDlgBar.GetDlgItemText(IDC_EDIT1, szText);
	pCmdUI->Enable(szText.GetLength() > 0);
}