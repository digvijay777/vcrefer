// HistoryListView.cpp : implementation file
//

#include "stdafx.h"
#include "BatchEmail.h"
#include "TaskListView.h"


// CHistoryListView

IMPLEMENT_DYNCREATE(CTaskListView, CListView)

CTaskListView::CTaskListView()
{
}

CTaskListView::~CTaskListView()
{
}

BEGIN_MESSAGE_MAP(CTaskListView, CListView)
END_MESSAGE_MAP()


// CHistoryListView diagnostics

#ifdef _DEBUG
void CTaskListView::AssertValid() const
{
	CListView::AssertValid();
}

void CTaskListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CHistoryListView message handlers
