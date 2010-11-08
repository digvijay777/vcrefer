// StudyGDI+Doc.cpp :  CStudyGDIDoc 类的实现
//

#include "stdafx.h"
#include "StudyGDI.h"

#include "StudyGDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudyGDIDoc

IMPLEMENT_DYNCREATE(CStudyGDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CStudyGDIDoc, CDocument)
END_MESSAGE_MAP()


// CStudyGDIDoc 构造/析构

CStudyGDIDoc::CStudyGDIDoc()
{
	// TODO: 在此添加一次性构造代码

}

CStudyGDIDoc::~CStudyGDIDoc()
{
}

BOOL CStudyGDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CStudyGDIDoc 序列化

void CStudyGDIDoc::Serialize(CArchive& ar)
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


// CStudyGDIDoc 诊断

#ifdef _DEBUG
void CStudyGDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStudyGDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStudyGDIDoc 命令
