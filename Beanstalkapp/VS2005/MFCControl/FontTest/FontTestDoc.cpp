// FontTestDoc.cpp : CFontTestDoc 类的实现
//

#include "stdafx.h"
#include "FontTest.h"

#include "FontTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFontTestDoc

IMPLEMENT_DYNCREATE(CFontTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CFontTestDoc, CDocument)
END_MESSAGE_MAP()


// CFontTestDoc 构造/析构

CFontTestDoc::CFontTestDoc()
{
	// TODO: 在此添加一次性构造代码

}

CFontTestDoc::~CFontTestDoc()
{
}

BOOL CFontTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CFontTestDoc 序列化

void CFontTestDoc::Serialize(CArchive& ar)
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


// CFontTestDoc 诊断

#ifdef _DEBUG
void CFontTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFontTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFontTestDoc 命令
