// ImgToolBarDoc.cpp :  CImgToolBarDoc ���ʵ��
//

#include "stdafx.h"
#include "ImgToolBar.h"

#include "ImgToolBarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgToolBarDoc

IMPLEMENT_DYNCREATE(CImgToolBarDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgToolBarDoc, CDocument)
END_MESSAGE_MAP()


// CImgToolBarDoc ����/����

CImgToolBarDoc::CImgToolBarDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CImgToolBarDoc::~CImgToolBarDoc()
{
}

BOOL CImgToolBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CImgToolBarDoc ���л�

void CImgToolBarDoc::Serialize(CArchive& ar)
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


// CImgToolBarDoc ���

#ifdef _DEBUG
void CImgToolBarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgToolBarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImgToolBarDoc ����
