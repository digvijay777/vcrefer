// testsdkDoc.cpp :  CtestsdkDoc 类的实现
//

#include "stdafx.h"
#include "testsdk.h"

#include "testsdkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestsdkDoc

IMPLEMENT_DYNCREATE(CtestsdkDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestsdkDoc, CDocument)
END_MESSAGE_MAP()


// CtestsdkDoc 构造/析构

CtestsdkDoc::CtestsdkDoc()
{
	// TODO: 在此添加一次性构造代码

}

CtestsdkDoc::~CtestsdkDoc()
{
}

BOOL CtestsdkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CtestsdkDoc 序列化

void CtestsdkDoc::Serialize(CArchive& ar)
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


// CtestsdkDoc 诊断

#ifdef _DEBUG
void CtestsdkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestsdkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestsdkDoc 命令
