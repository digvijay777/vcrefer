// ControlBarDoc.cpp :  CControlBarDoc ���ʵ��
//

#include "stdafx.h"
#include "ControlBar.h"

#include "ControlBarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlBarDoc

IMPLEMENT_DYNCREATE(CControlBarDoc, CDocument)

BEGIN_MESSAGE_MAP(CControlBarDoc, CDocument)
END_MESSAGE_MAP()


// CControlBarDoc ����/����

CControlBarDoc::CControlBarDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CControlBarDoc::~CControlBarDoc()
{
}

BOOL CControlBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CControlBarDoc ���л�

void CControlBarDoc::Serialize(CArchive& ar)
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


// CControlBarDoc ���

#ifdef _DEBUG
void CControlBarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CControlBarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CControlBarDoc ����
