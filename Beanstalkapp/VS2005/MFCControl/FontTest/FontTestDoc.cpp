// FontTestDoc.cpp : CFontTestDoc ���ʵ��
//

#include "stdafx.h"
#include "FontTest.h"

#include "FontTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFontTestDoc

IMPLEMENT_DYNCREATE(CFontTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CFontTestDoc, CDocument)
END_MESSAGE_MAP()


// CFontTestDoc ����/����

CFontTestDoc::CFontTestDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CFontTestDoc::~CFontTestDoc()
{
}

BOOL CFontTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CFontTestDoc ���л�

void CFontTestDoc::Serialize(CArchive& ar)
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


// CFontTestDoc ���

#ifdef _DEBUG
void CFontTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFontTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFontTestDoc ����
