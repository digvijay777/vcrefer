// DetoursDoc.cpp :  CDetoursDoc ���ʵ��
//

#include "stdafx.h"
#include "Detours.h"

#include "DetoursDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDetoursDoc

IMPLEMENT_DYNCREATE(CDetoursDoc, CDocument)

BEGIN_MESSAGE_MAP(CDetoursDoc, CDocument)
END_MESSAGE_MAP()


// CDetoursDoc ����/����

CDetoursDoc::CDetoursDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDetoursDoc::~CDetoursDoc()
{
}

BOOL CDetoursDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDetoursDoc ���л�

void CDetoursDoc::Serialize(CArchive& ar)
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


// CDetoursDoc ���

#ifdef _DEBUG
void CDetoursDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDetoursDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDetoursDoc ����
