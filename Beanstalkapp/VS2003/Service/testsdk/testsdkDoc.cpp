// testsdkDoc.cpp :  CtestsdkDoc ���ʵ��
//

#include "stdafx.h"
#include "testsdk.h"

#include "testsdkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestsdkDoc

IMPLEMENT_DYNCREATE(CtestsdkDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestsdkDoc, CDocument)
END_MESSAGE_MAP()


// CtestsdkDoc ����/����

CtestsdkDoc::CtestsdkDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CtestsdkDoc::~CtestsdkDoc()
{
}

BOOL CtestsdkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CtestsdkDoc ���л�

void CtestsdkDoc::Serialize(CArchive& ar)
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


// CtestsdkDoc ���

#ifdef _DEBUG
void CtestsdkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestsdkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestsdkDoc ����
