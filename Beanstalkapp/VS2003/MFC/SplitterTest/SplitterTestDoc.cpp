// SplitterTestDoc.cpp :  CSplitterTestDoc ���ʵ��
//

#include "stdafx.h"
#include "SplitterTest.h"

#include "SplitterTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSplitterTestDoc

IMPLEMENT_DYNCREATE(CSplitterTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CSplitterTestDoc, CDocument)
END_MESSAGE_MAP()


// CSplitterTestDoc ����/����

CSplitterTestDoc::CSplitterTestDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CSplitterTestDoc::~CSplitterTestDoc()
{
}

BOOL CSplitterTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSplitterTestDoc ���л�

void CSplitterTestDoc::Serialize(CArchive& ar)
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


// CSplitterTestDoc ���

#ifdef _DEBUG
void CSplitterTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSplitterTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSplitterTestDoc ����
