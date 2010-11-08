// SplitterTestDoc.cpp :  CSplitterTestDoc 类的实现
//

#include "stdafx.h"
#include "SplitterTest.h"

#include "SplitterTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSplitterTestDoc

IMPLEMENT_DYNCREATE(CSplitterTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CSplitterTestDoc, CDocument)
END_MESSAGE_MAP()


// CSplitterTestDoc 构造/析构

CSplitterTestDoc::CSplitterTestDoc()
{
	// TODO: 在此添加一次性构造代码

}

CSplitterTestDoc::~CSplitterTestDoc()
{
}

BOOL CSplitterTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSplitterTestDoc 序列化

void CSplitterTestDoc::Serialize(CArchive& ar)
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


// CSplitterTestDoc 诊断

#ifdef _DEBUG
void CSplitterTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSplitterTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSplitterTestDoc 命令
