// DetoursDoc.cpp :  CDetoursDoc 类的实现
//

#include "stdafx.h"
#include "Detours.h"

#include "DetoursDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDetoursDoc

IMPLEMENT_DYNCREATE(CDetoursDoc, CDocument)

BEGIN_MESSAGE_MAP(CDetoursDoc, CDocument)
END_MESSAGE_MAP()


// CDetoursDoc 构造/析构

CDetoursDoc::CDetoursDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDetoursDoc::~CDetoursDoc()
{
}

BOOL CDetoursDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDetoursDoc 序列化

void CDetoursDoc::Serialize(CArchive& ar)
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


// CDetoursDoc 诊断

#ifdef _DEBUG
void CDetoursDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDetoursDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDetoursDoc 命令
