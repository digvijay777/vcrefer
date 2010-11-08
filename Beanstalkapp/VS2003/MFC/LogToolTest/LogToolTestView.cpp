// LogToolTestView.cpp : CLogToolTestView ���ʵ��
//

#include "stdafx.h"
#include "LogToolTest.h"

#include "LogToolTestDoc.h"
#include "LogToolTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogToolTestView

IMPLEMENT_DYNCREATE(CLogToolTestView, CView)

BEGIN_MESSAGE_MAP(CLogToolTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CLogToolTestView ����/����

CLogToolTestView::CLogToolTestView()
{
	// TODO: �ڴ˴���ӹ������

}

CLogToolTestView::~CLogToolTestView()
{
}

BOOL CLogToolTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CLogToolTestView ����

void CLogToolTestView::OnDraw(CDC* /*pDC*/)
{
	CLogToolTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CLogToolTestView ��ӡ

BOOL CLogToolTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CLogToolTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CLogToolTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CLogToolTestView ���

#ifdef _DEBUG
void CLogToolTestView::AssertValid() const
{
	CView::AssertValid();
}

void CLogToolTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLogToolTestDoc* CLogToolTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogToolTestDoc)));
	return (CLogToolTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CLogToolTestView ��Ϣ�������
