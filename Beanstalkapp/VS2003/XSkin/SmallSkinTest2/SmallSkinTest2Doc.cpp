// SmallSkinTest2Doc.cpp :  CSmallSkinTest2Doc 类的实现
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


// CSmallSkinTest2Doc 构造/析构

CSmallSkinTest2Doc::CSmallSkinTest2Doc()
{
	// TODO: 在此添加一次性构造代码

}

CSmallSkinTest2Doc::~CSmallSkinTest2Doc()
{
}

BOOL CSmallSkinTest2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSmallSkinTest2Doc 序列化

void CSmallSkinTest2Doc::Serialize(CArchive& ar)
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


// CSmallSkinTest2Doc 诊断

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


// CSmallSkinTest2Doc 命令
