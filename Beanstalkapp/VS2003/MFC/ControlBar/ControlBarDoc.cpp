// ControlBarDoc.cpp :  CControlBarDoc 类的实现
//

#include "stdafx.h"
#include "ControlBar.h"

#include "ControlBarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlBarDoc

IMPLEMENT_DYNCREATE(CControlBarDoc, CDocument)

BEGIN_MESSAGE_MAP(CControlBarDoc, CDocument)
END_MESSAGE_MAP()


// CControlBarDoc 构造/析构

CControlBarDoc::CControlBarDoc()
{
	// TODO: 在此添加一次性构造代码

}

CControlBarDoc::~CControlBarDoc()
{
}

BOOL CControlBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CControlBarDoc 序列化

void CControlBarDoc::Serialize(CArchive& ar)
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


// CControlBarDoc 诊断

#ifdef _DEBUG
void CControlBarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CControlBarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CControlBarDoc 命令
