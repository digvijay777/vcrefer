// DetoursView.cpp : CDetoursView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

// CDetoursView ����/����

CDetoursView::CDetoursView()
{
	// TODO: �ڴ˴���ӹ������

}

CDetoursView::~CDetoursView()
{
}

BOOL CDetoursView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CHtmlView::PreCreateWindow(cs);
}

void CDetoursView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	Navigate2(_T("about:blank"),NULL,NULL);
}


// CDetoursView ��ӡ



// CDetoursView ���

#ifdef _DEBUG
void CDetoursView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CDetoursView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CDetoursDoc* CDetoursView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDetoursDoc)));
	return (CDetoursDoc*)m_pDocument;
}
#endif //_DEBUG


// CDetoursView ��Ϣ�������
