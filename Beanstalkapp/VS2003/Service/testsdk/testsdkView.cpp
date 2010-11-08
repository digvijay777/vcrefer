// testsdkView.cpp : CtestsdkView 类的实现
//

#include "stdafx.h"
#include "testsdk.h"

#include "testsdkDoc.h"
#include "testsdkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestsdkView

IMPLEMENT_DYNCREATE(CtestsdkView, CView)

BEGIN_MESSAGE_MAP(CtestsdkView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CtestsdkView 构造/析构

CtestsdkView::CtestsdkView()
{
	// TODO: 在此处添加构造代码

}

CtestsdkView::~CtestsdkView()
{
}

BOOL CtestsdkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CtestsdkView 绘制

void CtestsdkView::OnDraw(CDC* /*pDC*/)
{
	CtestsdkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CtestsdkView 打印

BOOL CtestsdkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CtestsdkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CtestsdkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CtestsdkView 诊断

#ifdef _DEBUG
void CtestsdkView::AssertValid() const
{
	CView::AssertValid();
}

void CtestsdkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtestsdkDoc* CtestsdkView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestsdkDoc)));
	return (CtestsdkDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestsdkView 消息处理程序
