// LicenseManagerView.cpp : CLicenseManagerView ���ʵ��
//

#include "stdafx.h"
#include "LicenseManager.h"

#include "LicenseManagerDoc.h"
#include "LicenseManagerView.h"
#include ".\licensemanagerview.h"
// #include "../Common/NSimple.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLicenseManagerView

IMPLEMENT_DYNCREATE(CLicenseManagerView, CView)

BEGIN_MESSAGE_MAP(CLicenseManagerView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CLicenseManagerView ����/����

CLicenseManagerView::CLicenseManagerView()
{
	// TODO: �ڴ˴���ӹ������

}

CLicenseManagerView::~CLicenseManagerView()
{
}

BOOL CLicenseManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CLicenseManagerView ����

void CLicenseManagerView::OnDraw(CDC* /*pDC*/)
{
	CLicenseManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CLicenseManagerView ��ӡ

BOOL CLicenseManagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CLicenseManagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CLicenseManagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CLicenseManagerView ���

#ifdef _DEBUG
void CLicenseManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CLicenseManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLicenseManagerDoc* CLicenseManagerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLicenseManagerDoc)));
	return (CLicenseManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLicenseManagerView ��Ϣ�������

int CLicenseManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CLicenseManagerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CLicenseManagerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}
