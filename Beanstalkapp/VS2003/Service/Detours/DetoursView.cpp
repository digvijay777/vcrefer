// DetoursView.cpp : CDetoursView 类的实现
//

#include "stdafx.h"
#include "Detours.h"

#include "DetoursDoc.h"
#include "DetoursView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDetoursView

IMPLEMENT_DYNCREATE(CDetoursView, CHtmlView)

BEGIN_MESSAGE_MAP(CDetoursView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

// CDetoursView 构造/析构

CDetoursView::CDetoursView()
{
	// TODO: 在此处添加构造代码

}

CDetoursView::~CDetoursView()
{
}

BOOL CDetoursView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CHtmlView::PreCreateWindow(cs);
}

void CDetoursView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	Navigate2(_T("about:blank"),NULL,NULL);
}


// CDetoursView 打印



// CDetoursView 诊断

#ifdef _DEBUG
void CDetoursView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CDetoursView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CDetoursDoc* CDetoursView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDetoursDoc)));
	return (CDetoursDoc*)m_pDocument;
}
#endif //_DEBUG


// CDetoursView 消息处理程序
