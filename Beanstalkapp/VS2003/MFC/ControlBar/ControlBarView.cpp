// ControlBarView.cpp : CControlBarView ���ʵ��
//

#include "stdafx.h"
#include "ControlBar.h"

#include "ControlBarDoc.h"
#include "ControlBarView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlBarView

IMPLEMENT_DYNCREATE(CControlBarView, CView)

BEGIN_MESSAGE_MAP(CControlBarView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(IDC_BT_TEST1, OnClickTest1)
	ON_UPDATE_COMMAND_UI(IDC_BT_TEST1, OnUpdateTest1)
END_MESSAGE_MAP()

// CControlBarView ����/����

CControlBarView::CControlBarView()
{
	// TODO: �ڴ˴���ӹ������

}

CControlBarView::~CControlBarView()
{
}

BOOL CControlBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CControlBarView ����

void CControlBarView::OnDraw(CDC* /*pDC*/)
{
	CControlBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CControlBarView ��ӡ

BOOL CControlBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CControlBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CControlBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CControlBarView ���

#ifdef _DEBUG
void CControlBarView::AssertValid() const
{
	CView::AssertValid();
}

void CControlBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CControlBarDoc* CControlBarView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CControlBarDoc)));
	return (CControlBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CControlBarView ��Ϣ�������
void CControlBarView::OnClickTest1()
{
	CMainFrame*		pFrame = (CMainFrame *)GetParent();
	if(NULL == pFrame)
		return;

	CString		szText;
	pFrame->m_wndDlgBar.GetDlgItemText(IDC_EDIT1, szText);
	AfxMessageBox(szText);
}

void CControlBarView::OnUpdateTest1(CCmdUI *pCmdUI)
{
	CMainFrame*		pFrame = (CMainFrame *)GetParent();
	if(NULL == pFrame)
		return;

	CString		szText;
	pFrame->m_wndDlgBar.GetDlgItemText(IDC_EDIT1, szText);
	pCmdUI->Enable(szText.GetLength() > 0);
}