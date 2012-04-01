// RichTextEx.cpp : implementation file
//

#include "stdafx.h"
#include "RichTextEx.h"


BOOL CRichTextEx::s_init		= FALSE;
// CRichTextEx
IMPLEMENT_DYNAMIC(CRichTextEx, CRichEditCtrl)

CRichTextEx::CRichTextEx()
{
	if(FALSE == s_init)
	{
		s_init = AfxInitRichEdit2();
		ASSERT(FALSE != s_init);
	}
}

CRichTextEx::~CRichTextEx()
{
}


BEGIN_MESSAGE_MAP(CRichTextEx, CRichEditCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CRichTextEx message handlers



void CRichTextEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// CRichEditCtrl::OnLButtonDown(nFlags, point);
}
