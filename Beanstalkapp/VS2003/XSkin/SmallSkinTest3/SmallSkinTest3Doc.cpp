// SmallSkinTest3Doc.cpp :  CSmallSkinTest3Doc 类的实现
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


// CSmallSkinTest3Doc 构造/析构

CSmallSkinTest3Doc::CSmallSkinTest3Doc()
{
	// TODO: 在此添加一次性构造代码

}

CSmallSkinTest3Doc::~CSmallSkinTest3Doc()
{
}

BOOL CSmallSkinTest3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSmallSkinTest3Doc 序列化

void CSmallSkinTest3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CSmallSkinTest3Doc 诊断

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


// CSmallSkinTest3Doc 命令
