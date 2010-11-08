// LicenseManagerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "LicenseManagerDoc.h"
#include ".\licensemanagerdoc.h"

// CLicenseManagerDoc

IMPLEMENT_DYNCREATE(CLicenseManagerDoc, CDocument)

CLicenseManagerDoc::CLicenseManagerDoc()
{
}

BOOL CLicenseManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CLicenseManagerDoc::~CLicenseManagerDoc()
{
}


BEGIN_MESSAGE_MAP(CLicenseManagerDoc, CDocument)
	ON_COMMAND(IDM_REFURBISH, OnRefurbish)
END_MESSAGE_MAP()


// CLicenseManagerDoc diagnostics

#ifdef _DEBUG
void CLicenseManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLicenseManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLicenseManagerDoc serialization

void CLicenseManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CLicenseManagerDoc commands
void CLicenseManagerDoc::NotifyView(UINT msg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	CFrameWnd*		pFrame	= (CFrameWnd *)AfxGetMainWnd();
	CView*			pView	= NULL;
	
	if(NULL == pFrame)
		return;
	pView = pFrame->GetActiveView();
	if(NULL == pView)
		return;
	pView->SendMessage(msg, wParam, lParam);
}
void CLicenseManagerDoc::OnRefurbish()
{
	NotifyView(UM_UPDATESHOW, 200);
}
