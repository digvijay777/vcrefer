/************************************************************************/
/*                                                                      */
/*                         XSkinDialog��                                */
/*                                                                      */
/*  �Ի���Ƥ����                                                        */
/************************************************************************/

#include "stdafx.h"
#include "XSkinDialog.h"
#include ".\xskindialog.h"

#ifndef DLG_RGB_DEFINE
#define RGB_EDGEOUTER			RGB(0x84, 0x9B, 0xBA)		// ���Ե��ɫ
#define RGB_EDGEINNER			RGB(0xE0, 0xE6, 0xEB)		// �ڱ�Ե��ɫ
#define RGB_BACKGROUND			RGB(0xC9, 0xD5, 0xE4)		// ���屳��ɫ			
#endif

IMPLEMENT_DYNAMIC(CXSkinDialog, CDialog)

CXSkinDialog::CXSkinDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_brushDlgBk.CreateSolidBrush(RGB_BACKGROUND);
	m_penEdgeOuter.CreatePen(PS_SOLID, 1, RGB_EDGEOUTER);
	m_penEdgeInner.CreatePen(PS_SOLID, 1, RGB_EDGEINNER);
	m_bNcLButtonDown = FALSE;
}

CXSkinDialog::~CXSkinDialog()
{
}



BEGIN_MESSAGE_MAP(CXSkinDialog, CDialog)
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()

LRESULT CXSkinDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_NCPAINT == message || WM_NCACTIVATE == message)
	{
		if(FALSE != DrawFrame())
		{
			Invalidate();
			UpdateWindow();
			return 0;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CXSkinDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if(FALSE != bCalcValidRects)
	{
// 		lpncsp->rgrc[0].top += 24 - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXFRAME);  
// 		lpncsp->rgrc[0].bottom -= 2 - GetSystemMetrics(SM_CYSIZEFRAME);   
// 		lpncsp->rgrc[0].left += 2 - GetSystemMetrics(SM_CXFRAME);   
// 		lpncsp->rgrc[0].right -= 2 - GetSystemMetrics(SM_CXFRAME); 
	}

	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CXSkinDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect		cRect;
	CRgn		cRgn;

	GetWindowRect(&cRect);
	cRect.MoveToXY(0, 0);
	cRgn.CreateRoundRectRgn(cRect.left, cRect.top, cRect.right, cRect.bottom, 3, 3);
	SetWindowRgn(cRgn, TRUE);
	cRgn.DeleteObject();
}

/*
 *	���Ʊ߿�
 *
 *  ���ʱ��:2009-11-9
 */
BOOL CXSkinDialog::DrawFrame()
{
	BOOL			bRet		= TRUE;
	CWindowDC		dc(this);
	CRect			cWndRect;
	CPen*			pOldPen		= NULL;
	CBrush*			pOldBrush	= NULL;

	pOldPen = dc.SelectObject(&m_penEdgeOuter);
	pOldBrush = dc.SelectObject(&m_brushDlgBk);

	GetWindowRect(&cWndRect);
	cWndRect.MoveToXY(0, 0);
	cWndRect.DeflateRect(0, 0, 1, 1);
 	dc.Rectangle(&cWndRect);	// ��߿�
	dc.SelectObject(&m_penEdgeInner);
	cWndRect.DeflateRect(1, 1, 1, 1);
	dc.Rectangle(&cWndRect);	// �ڱ߿�
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

	// ���Ʊ�����
	DrawTitleBar(&dc);
	return bRet;
}

/*
 *	���Ʊ�����
 *
 *  ���ʱ��:2009-11-9
 */
void CXSkinDialog::DrawTitleBar(CDC* pDC, DWORD nFlag /* = 0x0 */)
{
	CRect		cRect;

	GetWindowRect(&cRect);
	cRect.DeflateRect(2, 0, 2, 0);
	cRect.MoveToXY(2, 2);
	cRect.bottom = cRect.top + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXFRAME);
	pDC->FillRect(&cRect, &m_brushDlgBk);
}

BOOL CXSkinDialog::OnEraseBkgnd(CDC* pDC)
{
	CRect		cRect;

	GetWindowRect(&cRect);
// 	ScreenToClient(&cRect);
	cRect.DeflateRect(2, 24, 2, 2);
	cRect.MoveToXY(0, 0);
	pDC->FillRect(&cRect, &m_brushDlgBk);
	return FALSE;
}

void CXSkinDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//Get
	m_bNcLButtonDown = TRUE;
//	return;
	switch(nHitTest)
	{
	case HTCLOSE:
	case HTMAXBUTTON:
	case HTMINBUTTON:
		return;
	}
	DrawTitleBar(GetWindowDC());
	CDialog::OnNcLButtonDown(nHitTest, point);
	DrawTitleBar(GetWindowDC());
}

void CXSkinDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CXSkinDialog::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialog::OnNcMouseMove(nHitTest, point);
}
