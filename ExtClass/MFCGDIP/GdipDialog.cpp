// GdipDailog.cpp : implementation file
//

#include "stdafx.h"
#include "GdipDialog.h"

// CGdipDailog dialog

IMPLEMENT_DYNAMIC(CGdipDialog, CDialog)
CGdipDialog::CGdipDialog(UINT nIDTemplate, CWnd* pParent /* = NULL */)
	: CDialog(nIDTemplate)
{
	m_bBkgndChanged = TRUE;
}

CGdipDialog::~CGdipDialog()
{
	if(NULL != m_brushBkgnd.GetSafeHandle())
		m_brushBkgnd.DeleteObject();
}

void CGdipDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGdipDialog, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CGdipDailog message handlers

BOOL CGdipDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGdipDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH			hbr;

	// 背景改变
	if(FALSE != m_bBkgndChanged)
	{
		CreateBkgndBrush(pDC, &m_brushBkgnd);
		m_bBkgndChanged = FALSE;
		SetProp(GetSafeHwnd(), _T("BrushBkgnd"), (HANDLE)m_brushBkgnd.GetSafeHandle());		
	}

	if(NULL == m_brushBkgnd.GetSafeHandle())
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 改变某些控件的背景
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		hbr = (HBRUSH)m_brushBkgnd.GetSafeHandle();
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor((COLORREF)((DWORD)GetProp(pWnd->GetSafeHwnd(), _T("TextColor")) & 0xffffff));
		break;
	case CTLCOLOR_DLG:
		hbr = (HBRUSH)m_brushBkgnd.GetSafeHandle();
		break;
	case CTLCOLOR_MSGBOX:
		hbr = (HBRUSH)m_brushBkgnd.GetSafeHandle();
		break;
	default:
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	return hbr;
}

// 创建画刷
BOOL CGdipDialog::CreateBkgndBrush(CDC* pDC, CBrush* pBrush)
{
	if(NULL == pBrush)
	{
		ASSERT(NULL == pBrush);
		return FALSE;
	}

	if(NULL != pBrush->GetSafeHandle())
	{
		pBrush->DeleteObject();
	}

	CDC			memDC;
	CBitmap		bitmap;
	CBitmap*	pObject;
	CRect		rect;

	GetClientRect(&rect);
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pObject = memDC.SelectObject(&bitmap);
	SendMessage(WM_ERASEBKGND, (WPARAM)memDC.GetSafeHdc());
	memDC.SelectObject(pObject);
	pBrush->CreatePatternBrush(&bitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	return TRUE;
}