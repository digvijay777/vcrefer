// SmallSkinTest2Doc.cpp :  CSmallSkinTest2Doc ���ʵ��
//

#include "stdafx.h"
#include "SmallSkinTest2.h"

#include "SmallSkinTest2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmallSkinTest2Doc

IMPLEMENT_DYNCREATE(CSmallSkinTest2Doc, CDocument)

BEGIN_MESSAGE_MAP(CSmallSkinTest2Doc, CDocument)
END_MESSAGE_MAP()


// CSmallSkinTest2Doc ����/����

CSmallSkinTest2Doc::CSmallSkinTest2Doc()
{
	// TODO: �ڴ����һ���Թ������

}

CSmallSkinTest2Doc::~CSmallSkinTest2Doc()
{
}

BOOL CSmallSkinTest2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSmallSkinTest2Doc ���л�

void CSmallSkinTest2Doc::Serialize(CArchive& ar)
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


// CSmallSkinTest2Doc ���

#ifdef _DEBUG
void CSmallSkinTest2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSmallSkinTest2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSmallSkinTest2Doc ����
