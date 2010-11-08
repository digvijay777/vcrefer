// testsdkView.cpp : CtestsdkView ���ʵ��
//

#include "stdafx.h"
#include "testsdk.h"

#include "testsdkDoc.h"
#include "testsdkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestsdkView

IMPLEMENT_DYNCREATE(CtestsdkView, CView)

BEGIN_MESSAGE_MAP(CtestsdkView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CtestsdkView ����/����

CtestsdkView::CtestsdkView()
{
	// TODO: �ڴ˴���ӹ������

}

CtestsdkView::~CtestsdkView()
{
}

BOOL CtestsdkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CtestsdkView ����

void CtestsdkView::OnDraw(CDC* /*pDC*/)
{
	CtestsdkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CtestsdkView ��ӡ

BOOL CtestsdkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CtestsdkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CtestsdkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CtestsdkView ���

#ifdef _DEBUG
void CtestsdkView::AssertValid() const
{
	CView::AssertValid();
}

void CtestsdkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtestsdkDoc* CtestsdkView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestsdkDoc)));
	return (CtestsdkDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestsdkView ��Ϣ�������
