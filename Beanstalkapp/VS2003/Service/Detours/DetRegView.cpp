// DetRegView.cpp : 实现文件
//

#include "stdafx.h"
#include "Detours.h"
#include "DetRegView.h"


// CDetRegView

IMPLEMENT_DYNCREATE(CDetRegView, CListView)

CDetRegView::CDetRegView()
{
}

CDetRegView::~CDetRegView()
{
}

BEGIN_MESSAGE_MAP(CDetRegView, CListView)
END_MESSAGE_MAP()


// CDetRegView 诊断

#ifdef _DEBUG
void CDetRegView::AssertValid() const
{
	CListView::AssertValid();
}

void CDetRegView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CDetRegView 消息处理程序
