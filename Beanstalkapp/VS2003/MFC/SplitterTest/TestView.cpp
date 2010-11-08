// TestView.cpp : implementation file
//

#include "stdafx.h"
#include "SplitterTest.h"
#include "TestView.h"
#include ".\testview.h"


// CTestView

IMPLEMENT_DYNCREATE(CTestView, CFormView)

CTestView::CTestView()
	: CFormView(CTestView::IDD)
{
}

CTestView::~CTestView()
{
}

void CTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CTestView message handlers

void CTestView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
