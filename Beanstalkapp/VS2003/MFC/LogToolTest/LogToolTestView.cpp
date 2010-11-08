// LogToolTestView.cpp : CLogToolTestView 类的实现
//

#include "stdafx.h"
#include "LogToolTest.h"

#include "LogToolTestDoc.h"
#include "LogToolTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogToolTestView

IMPLEMENT_DYNCREATE(CLogToolTestView, CView)

BEGIN_MESSAGE_MAP(CLogToolTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CLogToolTestView 构造/析构

CLogToolTestView::CLogToolTestView()
{
	// TODO: 在此处添加构造代码

}

CLogToolTestView::~CLogToolTestView()
{
}

BOOL CLogToolTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CLogToolTestView 绘制

void CLogToolTestView::OnDraw(CDC* /*pDC*/)
{
	CLogToolTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CLogToolTestView 打印

BOOL CLogToolTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLogToolTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CLogToolTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CLogToolTestView 诊断

#ifdef _DEBUG
void CLogToolTestView::AssertValid() const
{
	CView::AssertValid();
}

void CLogToolTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLogToolTestDoc* CLogToolTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogToolTestDoc)));
	return (CLogToolTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CLogToolTestView 消息处理程序
