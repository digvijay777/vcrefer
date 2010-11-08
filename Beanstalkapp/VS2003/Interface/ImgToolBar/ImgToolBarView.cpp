// ImgToolBarView.cpp : CImgToolBarView 类的实现
//

#include "stdafx.h"
#include "ImgToolBar.h"

#include "ImgToolBarDoc.h"
#include "ImgToolBarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgToolBarView

IMPLEMENT_DYNCREATE(CImgToolBarView, CView)

BEGIN_MESSAGE_MAP(CImgToolBarView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CImgToolBarView 构造/析构

CImgToolBarView::CImgToolBarView()
{
	// TODO: 在此处添加构造代码

}

CImgToolBarView::~CImgToolBarView()
{
}

BOOL CImgToolBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CImgToolBarView 绘制

void CImgToolBarView::OnDraw(CDC* /*pDC*/)
{
	CImgToolBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CImgToolBarView 打印

BOOL CImgToolBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImgToolBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CImgToolBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CImgToolBarView 诊断

#ifdef _DEBUG
void CImgToolBarView::AssertValid() const
{
	CView::AssertValid();
}

void CImgToolBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImgToolBarDoc* CImgToolBarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgToolBarDoc)));
	return (CImgToolBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CImgToolBarView 消息处理程序
