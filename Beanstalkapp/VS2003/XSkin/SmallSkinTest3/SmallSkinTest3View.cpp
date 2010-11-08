// SmallSkinTest3View.cpp : CSmallSkinTest3View 类的实现
//

#include "stdafx.h"
#include "SmallSkinTest3.h"

#include "SmallSkinTest3Doc.h"
#include "SmallSkinTest3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmallSkinTest3View

IMPLEMENT_DYNCREATE(CSmallSkinTest3View, CView)

BEGIN_MESSAGE_MAP(CSmallSkinTest3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSmallSkinTest3View 构造/析构

CSmallSkinTest3View::CSmallSkinTest3View()
{
	// TODO: 在此处添加构造代码

}

CSmallSkinTest3View::~CSmallSkinTest3View()
{
}

BOOL CSmallSkinTest3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CSmallSkinTest3View 绘制

void CSmallSkinTest3View::OnDraw(CDC* /*pDC*/)
{
	CSmallSkinTest3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CSmallSkinTest3View 打印

BOOL CSmallSkinTest3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSmallSkinTest3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CSmallSkinTest3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CSmallSkinTest3View 诊断

#ifdef _DEBUG
void CSmallSkinTest3View::AssertValid() const
{
	CView::AssertValid();
}

void CSmallSkinTest3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmallSkinTest3Doc* CSmallSkinTest3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmallSkinTest3Doc)));
	return (CSmallSkinTest3Doc*)m_pDocument;
}
#endif //_DEBUG


// CSmallSkinTest3View 消息处理程序
