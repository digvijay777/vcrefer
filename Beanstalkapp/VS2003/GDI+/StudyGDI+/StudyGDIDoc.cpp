// StudyGDI+Doc.cpp :  CStudyGDIDoc ���ʵ��
//

#include "stdafx.h"
#include "StudyGDI.h"

#include "StudyGDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudyGDIDoc

IMPLEMENT_DYNCREATE(CStudyGDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CStudyGDIDoc, CDocument)
END_MESSAGE_MAP()


// CStudyGDIDoc ����/����

CStudyGDIDoc::CStudyGDIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CStudyGDIDoc::~CStudyGDIDoc()
{
}

BOOL CStudyGDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CStudyGDIDoc ���л�

void CStudyGDIDoc::Serialize(CArchive& ar)
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


// CStudyGDIDoc ���

#ifdef _DEBUG
void CStudyGDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStudyGDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStudyGDIDoc ����
