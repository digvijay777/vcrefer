// SmallSkinTest3View.cpp : CSmallSkinTest3View ���ʵ��
//

#include "stdafx.h"
#include "SmallSkinTest3.h"

#include "SmallSkinTest3Doc.h"
#include "SmallSkinTest3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmallSkinTest3View

IMPLEMENT_DYNCREATE(CSmallSkinTest3View, CView)

BEGIN_MESSAGE_MAP(CSmallSkinTest3View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSmallSkinTest3View ����/����

CSmallSkinTest3View::CSmallSkinTest3View()
{
	// TODO: �ڴ˴���ӹ������

}

CSmallSkinTest3View::~CSmallSkinTest3View()
{
}

BOOL CSmallSkinTest3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CSmallSkinTest3View ����

void CSmallSkinTest3View::OnDraw(CDC* /*pDC*/)
{
	CSmallSkinTest3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CSmallSkinTest3View ��ӡ

BOOL CSmallSkinTest3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSmallSkinTest3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CSmallSkinTest3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CSmallSkinTest3View ���

#ifdef _DEBUG
void CSmallSkinTest3View::AssertValid() const
{
	CView::AssertValid();
}

void CSmallSkinTest3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmallSkinTest3Doc* CSmallSkinTest3View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmallSkinTest3Doc)));
	return (CSmallSkinTest3Doc*)m_pDocument;
}
#endif //_DEBUG


// CSmallSkinTest3View ��Ϣ�������
