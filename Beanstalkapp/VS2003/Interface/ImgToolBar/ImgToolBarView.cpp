// ImgToolBarView.cpp : CImgToolBarView ���ʵ��
//

#include "stdafx.h"
#include "ImgToolBar.h"

#include "ImgToolBarDoc.h"
#include "ImgToolBarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgToolBarView

IMPLEMENT_DYNCREATE(CImgToolBarView, CView)

BEGIN_MESSAGE_MAP(CImgToolBarView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CImgToolBarView ����/����

CImgToolBarView::CImgToolBarView()
{
	// TODO: �ڴ˴���ӹ������

}

CImgToolBarView::~CImgToolBarView()
{
}

BOOL CImgToolBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CImgToolBarView ����

void CImgToolBarView::OnDraw(CDC* /*pDC*/)
{
	CImgToolBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CImgToolBarView ��ӡ

BOOL CImgToolBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImgToolBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CImgToolBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CImgToolBarView ���

#ifdef _DEBUG
void CImgToolBarView::AssertValid() const
{
	CView::AssertValid();
}

void CImgToolBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImgToolBarDoc* CImgToolBarView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgToolBarDoc)));
	return (CImgToolBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CImgToolBarView ��Ϣ�������
