// EHomeListsDoc.cpp : CEHomeListsDoc ���ʵ��
//

#include "stdafx.h"
#include "EHomeLists.h"

#include "EHomeListsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEHomeListsDoc

IMPLEMENT_DYNCREATE(CEHomeListsDoc, CDocument)

BEGIN_MESSAGE_MAP(CEHomeListsDoc, CDocument)
END_MESSAGE_MAP()


// CEHomeListsDoc ����/����

CEHomeListsDoc::CEHomeListsDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CEHomeListsDoc::~CEHomeListsDoc()
{
}

BOOL CEHomeListsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CEHomeListsDoc ���л�

void CEHomeListsDoc::Serialize(CArchive& ar)
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


// CEHomeListsDoc ���

#ifdef _DEBUG
void CEHomeListsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEHomeListsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEHomeListsDoc ����
