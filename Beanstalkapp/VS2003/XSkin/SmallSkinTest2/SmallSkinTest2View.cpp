// SmallSkinTest2View.cpp : CSmallSkinTest2View ���ʵ��
//

#include "stdafx.h"
#include "SmallSkinTest2.h"

#include "SmallSkinTest2Doc.h"
#include "SmallSkinTest2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmallSkinTest2View

IMPLEMENT_DYNCREATE(CSmallSkinTest2View, CView)

BEGIN_MESSAGE_MAP(CSmallSkinTest2View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSmallSkinTest2View ����/����

CSmallSkinTest2View::CSmallSkinTest2View()
{
	// TODO: �ڴ˴���ӹ������

}

CSmallSkinTest2View::~CSmallSkinTest2View()
{
}

BOOL CSmallSkinTest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CSmallSkinTest2View ����

void CSmallSkinTest2View::OnDraw(CDC* /*pDC*/)
{
	CSmallSkinTest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CSmallSkinTest2View ��ӡ

BOOL CSmallSkinTest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSmallSkinTest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CSmallSkinTest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CSmallSkinTest2View ���

#ifdef _DEBUG
void CSmallSkinTest2View::AssertValid() const
{
	CView::AssertValid();
}

void CSmallSkinTest2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmallSkinTest2Doc* CSmallSkinTest2View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmallSkinTest2Doc)));
	return (CSmallSkinTest2Doc*)m_pDocument;
}
#endif //_DEBUG


// CSmallSkinTest2View ��Ϣ�������
