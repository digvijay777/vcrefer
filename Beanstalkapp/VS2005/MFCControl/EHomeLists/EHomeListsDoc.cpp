// EHomeListsDoc.cpp : CEHomeListsDoc 类的实现
//

#include "stdafx.h"
#include "EHomeLists.h"

#include "EHomeListsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEHomeListsDoc

IMPLEMENT_DYNCREATE(CEHomeListsDoc, CDocument)

BEGIN_MESSAGE_MAP(CEHomeListsDoc, CDocument)
END_MESSAGE_MAP()


// CEHomeListsDoc 构造/析构

CEHomeListsDoc::CEHomeListsDoc()
{
	// TODO: 在此添加一次性构造代码

}

CEHomeListsDoc::~CEHomeListsDoc()
{
}

BOOL CEHomeListsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CEHomeListsDoc 序列化

void CEHomeListsDoc::Serialize(CArchive& ar)
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


// CEHomeListsDoc 诊断

#ifdef _DEBUG
void CEHomeListsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEHomeListsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEHomeListsDoc 命令
