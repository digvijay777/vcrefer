// LogToolTestDoc.cpp :  CLogToolTestDoc ���ʵ��
//

#include "stdafx.h"
#include "LogToolTest.h"

#include "LogToolTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogToolTestDoc

IMPLEMENT_DYNCREATE(CLogToolTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogToolTestDoc, CDocument)
END_MESSAGE_MAP()


// CLogToolTestDoc ����/����

CLogToolTestDoc::CLogToolTestDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CLogToolTestDoc::~CLogToolTestDoc()
{
}

BOOL CLogToolTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CLogToolTestDoc ���л�

void CLogToolTestDoc::Serialize(CArchive& ar)
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


// CLogToolTestDoc ���

#ifdef _DEBUG
void CLogToolTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogToolTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLogToolTestDoc ����
