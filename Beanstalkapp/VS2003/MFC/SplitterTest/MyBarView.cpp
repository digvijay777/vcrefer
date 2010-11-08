// MyBarView.cpp : implementation file
//

#include "stdafx.h"
#include "SplitterTest.h"
#include "MyBarView.h"
#include ".\mybarview.h"
#include "TestView.h"

// CMyBarView

IMPLEMENT_DYNCREATE(CMyBarView, CBarView)

CMyBarView::CMyBarView()
{
}

CMyBarView::~CMyBarView()
{
}

BEGIN_MESSAGE_MAP(CMyBarView, CBarView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMyBarView drawing

void CMyBarView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CBarView::OnDraw(pDC);
}


// CMyBarView diagnostics

#ifdef _DEBUG
void CMyBarView::AssertValid() const
{
	CBarView::AssertValid();
}

void CMyBarView::Dump(CDumpContext& dc) const
{
	CBarView::Dump(dc);
}
#endif //_DEBUG


// CMyBarView message handlers

int CMyBarView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBarView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBarView::SetView(RUNTIME_CLASS(CTestView));
	CBarView::AddBarView(RUNTIME_CLASS(CTestView), "BAR 1");
	CBarView::AddBarView(RUNTIME_CLASS(CTestView), "BAR 2");

	return 0;
}
