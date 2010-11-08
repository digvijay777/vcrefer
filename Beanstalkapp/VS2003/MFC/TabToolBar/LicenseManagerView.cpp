// LicenseManagerView.cpp : CLicenseManagerView 类的实现
//

#include "stdafx.h"
#include "LicenseManager.h"

#include "LicenseManagerDoc.h"
#include "LicenseManagerView.h"
#include ".\licensemanagerview.h"
// #include "../Common/NSimple.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLicenseManagerView

IMPLEMENT_DYNCREATE(CLicenseManagerView, CView)

BEGIN_MESSAGE_MAP(CLicenseManagerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CLicenseManagerView 构造/析构

CLicenseManagerView::CLicenseManagerView()
{
	// TODO: 在此处添加构造代码

}

CLicenseManagerView::~CLicenseManagerView()
{
}

BOOL CLicenseManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CLicenseManagerView 绘制

void CLicenseManagerView::OnDraw(CDC* /*pDC*/)
{
	CLicenseManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CLicenseManagerView 打印

BOOL CLicenseManagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLicenseManagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CLicenseManagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CLicenseManagerView 诊断

#ifdef _DEBUG
void CLicenseManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CLicenseManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLicenseManagerDoc* CLicenseManagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLicenseManagerDoc)));
	return (CLicenseManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLicenseManagerView 消息处理程序

int CLicenseManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CLicenseManagerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CLicenseManagerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}
