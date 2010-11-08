// SmallSkinTest2View.cpp : CSmallSkinTest2View 类的实现
//

#include "stdafx.h"
#include "SmallSkinTest2.h"

#include "SmallSkinTest2Doc.h"
#include "SmallSkinTest2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmallSkinTest2View

IMPLEMENT_DYNCREATE(CSmallSkinTest2View, CView)

BEGIN_MESSAGE_MAP(CSmallSkinTest2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSmallSkinTest2View 构造/析构

CSmallSkinTest2View::CSmallSkinTest2View()
{
	// TODO: 在此处添加构造代码

}

CSmallSkinTest2View::~CSmallSkinTest2View()
{
}

BOOL CSmallSkinTest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CSmallSkinTest2View 绘制

void CSmallSkinTest2View::OnDraw(CDC* /*pDC*/)
{
	CSmallSkinTest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CSmallSkinTest2View 打印

BOOL CSmallSkinTest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSmallSkinTest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CSmallSkinTest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CSmallSkinTest2View 诊断

#ifdef _DEBUG
void CSmallSkinTest2View::AssertValid() const
{
	CView::AssertValid();
}

void CSmallSkinTest2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmallSkinTest2Doc* CSmallSkinTest2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmallSkinTest2Doc)));
	return (CSmallSkinTest2Doc*)m_pDocument;
}
#endif //_DEBUG


// CSmallSkinTest2View 消息处理程序
