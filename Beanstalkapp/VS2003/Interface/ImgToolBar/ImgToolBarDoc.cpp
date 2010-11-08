// ImgToolBarDoc.cpp :  CImgToolBarDoc 类的实现
//

#include "stdafx.h"
#include "ImgToolBar.h"

#include "ImgToolBarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgToolBarDoc

IMPLEMENT_DYNCREATE(CImgToolBarDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgToolBarDoc, CDocument)
END_MESSAGE_MAP()


// CImgToolBarDoc 构造/析构

CImgToolBarDoc::CImgToolBarDoc()
{
	// TODO: 在此添加一次性构造代码

}

CImgToolBarDoc::~CImgToolBarDoc()
{
}

BOOL CImgToolBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImgToolBarDoc 序列化

void CImgToolBarDoc::Serialize(CArchive& ar)
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


// CImgToolBarDoc 诊断

#ifdef _DEBUG
void CImgToolBarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgToolBarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImgToolBarDoc 命令
