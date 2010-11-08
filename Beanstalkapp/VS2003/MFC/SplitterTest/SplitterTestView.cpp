// SplitterTestView.cpp : CSplitterTestView ���ʵ��
//

#include "stdafx.h"
#include "SplitterTest.h"

#include "SplitterTestDoc.h"
#include "SplitterTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSplitterTestView

IMPLEMENT_DYNCREATE(CSplitterTestView, CView)

BEGIN_MESSAGE_MAP(CSplitterTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSplitterTestView ����/����

CSplitterTestView::CSplitterTestView()
{
	// TODO: �ڴ˴���ӹ������

}

CSplitterTestView::~CSplitterTestView()
{
}

BOOL CSplitterTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CSplitterTestView ����

void CSplitterTestView::OnDraw(CDC* /*pDC*/)
{
	CSplitterTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CSplitterTestView ��ӡ

BOOL CSplitterTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSplitterTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CSplitterTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CSplitterTestView ���

#ifdef _DEBUG
void CSplitterTestView::AssertValid() const
{
	CView::AssertValid();
}

void CSplitterTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSplitterTestDoc* CSplitterTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSplitterTestDoc)));
	return (CSplitterTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CSplitterTestView ��Ϣ�������
