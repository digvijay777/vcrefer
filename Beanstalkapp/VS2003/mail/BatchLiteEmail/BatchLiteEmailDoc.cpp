// BatchLiteEmailDoc.cpp :  CBatchLiteEmailDoc 类的实现
//

#include "stdafx.h"
#include "BatchLiteEmail.h"

#include "BatchLiteEmailDoc.h"
#include ".\batchliteemaildoc.h"

#include "LinkMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBatchLiteEmailDoc

IMPLEMENT_DYNCREATE(CBatchLiteEmailDoc, CDocument)

BEGIN_MESSAGE_MAP(CBatchLiteEmailDoc, CDocument)
	ON_COMMAND(IDM_LINKMAIN, OnLinkmain)
END_MESSAGE_MAP()


// CBatchLiteEmailDoc 构造/析构

CBatchLiteEmailDoc::CBatchLiteEmailDoc()
{
	// TODO: 在此添加一次性构造代码

}

CBatchLiteEmailDoc::~CBatchLiteEmailDoc()
{
}

BOOL CBatchLiteEmailDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CBatchLiteEmailDoc 序列化

void CBatchLiteEmailDoc::Serialize(CArchive& ar)
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


// CBatchLiteEmailDoc 诊断

#ifdef _DEBUG
void CBatchLiteEmailDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBatchLiteEmailDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBatchLiteEmailDoc 命令

BOOL CBatchLiteEmailDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
// 	if (!CDocument::OnOpenDocument(lpszPathName))
// 		return FALSE;

	// TODO:  Add your specialized creation code here
	((CHtmlView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView())->Navigate2(lpszPathName);
	theApp.AddToRecentFileList(lpszPathName);
	return TRUE;
}

void CBatchLiteEmailDoc::OnLinkmain()
{
	CLinkMainDlg		dlg;

	dlg.DoModal();
}
