// LogToolTestDoc.cpp :  CLogToolTestDoc 类的实现
//

#include "stdafx.h"
#include "LogToolTest.h"

#include "LogToolTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogToolTestDoc

IMPLEMENT_DYNCREATE(CLogToolTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogToolTestDoc, CDocument)
END_MESSAGE_MAP()


// CLogToolTestDoc 构造/析构

CLogToolTestDoc::CLogToolTestDoc()
{
	// TODO: 在此添加一次性构造代码

}

CLogToolTestDoc::~CLogToolTestDoc()
{
}

BOOL CLogToolTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CLogToolTestDoc 序列化

void CLogToolTestDoc::Serialize(CArchive& ar)
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


// CLogToolTestDoc 诊断

#ifdef _DEBUG
void CLogToolTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogToolTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLogToolTestDoc 命令
