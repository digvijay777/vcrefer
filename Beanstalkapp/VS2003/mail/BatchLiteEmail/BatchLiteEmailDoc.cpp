// BatchLiteEmailDoc.cpp :  CBatchLiteEmailDoc ���ʵ��
//

#include "stdafx.h"
#include "BatchLiteEmail.h"

#include "BatchLiteEmailDoc.h"
#include ".\batchliteemaildoc.h"

#include "LinkMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchLiteEmailDoc

IMPLEMENT_DYNCREATE(CBatchLiteEmailDoc, CDocument)

BEGIN_MESSAGE_MAP(CBatchLiteEmailDoc, CDocument)
	ON_COMMAND(IDM_LINKMAIN, OnLinkmain)
END_MESSAGE_MAP()


// CBatchLiteEmailDoc ����/����

CBatchLiteEmailDoc::CBatchLiteEmailDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CBatchLiteEmailDoc::~CBatchLiteEmailDoc()
{
}

BOOL CBatchLiteEmailDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CBatchLiteEmailDoc ���л�

void CBatchLiteEmailDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CBatchLiteEmailDoc ���

#ifdef _DEBUG
void CBatchLiteEmailDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBatchLiteEmailDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBatchLiteEmailDoc ����

BOOL CBatchLiteEmailDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
// 	if (!CDocument::OnOpenDocument(lpszPathName))
// 		return FALSE;

	// TODO:  Add your specialized creation code here
	((CHtmlView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView())->Navigate2(lpszPathName);
	theApp.AddToRecentFileList(lpszPathName);
	return TRUE;
}

void CBatchLiteEmailDoc::OnLinkmain()
{
	CLinkMainDlg		dlg;

	dlg.DoModal();
}
