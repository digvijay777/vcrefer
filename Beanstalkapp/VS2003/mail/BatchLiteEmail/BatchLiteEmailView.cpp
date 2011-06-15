// BatchLiteEmailView.cpp : CBatchLiteEmailView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
//	ON_COMMAND(IDM_SENDMAIL, OnSendmail)
ON_COMMAND(IDM_SENDMAIL, OnSendmail)
END_MESSAGE_MAP()

// CBatchLiteEmailView ����/����

CBatchLiteEmailView::CBatchLiteEmailView()
{
	// TODO: �ڴ˴���ӹ������

}

CBatchLiteEmailView::~CBatchLiteEmailView()
{
}

BOOL CBatchLiteEmailView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

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


// CBatchLiteEmailView ��ӡ



// CBatchLiteEmailView ���

#ifdef _DEBUG
void CBatchLiteEmailView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CBatchLiteEmailView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CBatchLiteEmailDoc* CBatchLiteEmailView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBatchLiteEmailDoc)));
	return (CBatchLiteEmailDoc*)m_pDocument;
}
#endif //_DEBUG


// CBatchLiteEmailView ��Ϣ�������

void CBatchLiteEmailView::OnFileOpen()
{
	// ���ļ�
	CFileDialog		fd(TRUE, "*.html;*.htm", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		, "��ҳ�ļ�(*.html,*.htm)|*.html;*.htm||", AfxGetMainWnd());
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
		MessageBox("��ѡ��Ҫ���͵���ҳ:" + strUrl);
		return;
	}

	CSendMailDlg		dlg;

	dlg.m_strTemplateUrl = strUrl;
	dlg.DoModal();
}
