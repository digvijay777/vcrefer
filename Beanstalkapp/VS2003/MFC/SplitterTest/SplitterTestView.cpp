// SplitterTestView.cpp : CSplitterTestView 类的实现
//

#include "stdafx.h"
#include "SplitterTest.h"

#include "SplitterTestDoc.h"
#include "SplitterTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSplitterTestView

IMPLEMENT_DYNCREATE(CSplitterTestView, CView)

BEGIN_MESSAGE_MAP(CSplitterTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSplitterTestView 构造/析构

CSplitterTestView::CSplitterTestView()
{
	// TODO: 在此处添加构造代码

}

CSplitterTestView::~CSplitterTestView()
{
}

BOOL CSplitterTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CSplitterTestView 绘制

void CSplitterTestView::OnDraw(CDC* /*pDC*/)
{
	CSplitterTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CSplitterTestView 打印

BOOL CSplitterTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSplitterTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CSplitterTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CSplitterTestView 诊断

#ifdef _DEBUG
void CSplitterTestView::AssertValid() const
{
	CView::AssertValid();
}

void CSplitterTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSplitterTestDoc* CSplitterTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSplitterTestDoc)));
	return (CSplitterTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CSplitterTestView 消息处理程序
