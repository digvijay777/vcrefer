// BatchLiteEmailView.cpp : CBatchLiteEmailView 类的实现
//

#include "stdafx.h"
#include "BatchLiteEmail.h"

#include "BatchLiteEmailDoc.h"
#include "BatchLiteEmailView.h"
#include ".\batchliteemailview.h"
#include "SendMailDlg.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "shlwapi.lib")
// CBatchLiteEmailView

IMPLEMENT_DYNCREATE(CBatchLiteEmailView, CHtmlView)

BEGIN_MESSAGE_MAP(CBatchLiteEmailView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
//	ON_COMMAND(IDM_SENDMAIL, OnSendmail)
ON_COMMAND(IDM_SENDMAIL, OnSendmail)
END_MESSAGE_MAP()

// CBatchLiteEmailView 构造/析构

CBatchLiteEmailView::CBatchLiteEmailView()
{
	// TODO: 在此处添加构造代码

}

CBatchLiteEmailView::~CBatchLiteEmailView()
{
}

BOOL CBatchLiteEmailView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CHtmlView::PreCreateWindow(cs);
}

void CBatchLiteEmailView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	//Navigate2(_T("http://www.msdn.microsoft.com/visualc/"),NULL,NULL);
	//Navigate2(GetHistory("URL", "about:blank"), NULL, NULL);
	if("" == GetLocationURL())
	{
		Navigate2(theApp.GetFirstRecentFile());
	}
}


// CBatchLiteEmailView 打印



// CBatchLiteEmailView 诊断

#ifdef _DEBUG
void CBatchLiteEmailView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CBatchLiteEmailView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CBatchLiteEmailDoc* CBatchLiteEmailView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBatchLiteEmailDoc)));
	return (CBatchLiteEmailDoc*)m_pDocument;
}
#endif //_DEBUG


// CBatchLiteEmailView 消息处理程序

void CBatchLiteEmailView::OnFileOpen()
{
	// 打开文件
	CFileDialog		fd(TRUE, "*.html;*.htm", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		, "网页文件(*.html,*.htm)|*.html;*.htm||", AfxGetMainWnd());
	if(IDOK == fd.DoModal())
	{
		Navigate2(fd.GetPathName(), NULL, NULL);
		theApp.AddToRecentFileList(fd.GetPathName());
	}
}

//void CBatchLiteEmailView::OnSendmail()
//{
//	// TODO: Add your command handler code here
//}

void CBatchLiteEmailView::OnSendmail()
{
	CString			strUrl;

	strUrl = GetLocationURL();
	if(strUrl.Left(8) == "file:///")
		strUrl = strUrl.Right(strUrl.GetLength()-8);
	if(FALSE == PathFileExists(strUrl))
	{
		MessageBox("请选择要发送的网页:" + strUrl);
		return;
	}

	CSendMailDlg		dlg;

	dlg.m_strTemplateUrl = strUrl;
	dlg.DoModal();
}
