// DetRegView.cpp : ʵ���ļ�
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


// CDetRegView ���

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


// CDetRegView ��Ϣ�������
