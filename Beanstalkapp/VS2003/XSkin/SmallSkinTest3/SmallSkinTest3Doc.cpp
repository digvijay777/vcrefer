// SmallSkinTest3Doc.cpp :  CSmallSkinTest3Doc ���ʵ��
//

#include "stdafx.h"
#include "SmallSkinTest3.h"

#include "SmallSkinTest3Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmallSkinTest3Doc

IMPLEMENT_DYNCREATE(CSmallSkinTest3Doc, CDocument)

BEGIN_MESSAGE_MAP(CSmallSkinTest3Doc, CDocument)
END_MESSAGE_MAP()


// CSmallSkinTest3Doc ����/����

CSmallSkinTest3Doc::CSmallSkinTest3Doc()
{
	// TODO: �ڴ����һ���Թ������

}

CSmallSkinTest3Doc::~CSmallSkinTest3Doc()
{
}

BOOL CSmallSkinTest3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSmallSkinTest3Doc ���л�

void CSmallSkinTest3Doc::Serialize(CArchive& ar)
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


// CSmallSkinTest3Doc ���

#ifdef _DEBUG
void CSmallSkinTest3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSmallSkinTest3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSmallSkinTest3Doc ����
