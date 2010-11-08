#include "stdafx.h"
#include "RadioButton.h"
#include ".\radiobutton.h"

CRadioButton::CRadioButton()
{

}

CRadioButton::~CRadioButton()
{

}

BEGIN_MESSAGE_MAP(CRadioButton, CButton)
	//{{AFX_MSG_MAP(CImageButton)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CRadioButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, 0);
	CButton::PreSubclassWindow();
}

/*
 *	设置单选按钮位置
 *
 *  位置说明,1位为未选,2位为选中态
 */
BOOL CRadioButton::LoadBitmap(UINT nID)
{
	BOOL		bRes;

	bRes = m_RadioBitmap.LoadBitmap(nID);
	m_RadioBitmap.GetBitmap(&m_RadioBmpInfo);

	return bRes;
}

/*
 *	绘制位图
 */
BOOL CRadioButton::DrawBitmap(CDC* pDC, CBitmap* pBmp, CRect& rtDC, CPoint& ptBmp)
{
	CDC			memDC;
	CBitmap*	pOldBmp;

	if(FALSE == memDC.CreateCompatibleDC(pDC))
		return FALSE;
	pOldBmp = memDC.SelectObject(pBmp);
	pDC->TransparentBlt(rtDC.left, rtDC.top, rtDC.Width(), rtDC.Height(), 
		&memDC, ptBmp.x, ptBmp.y, rtDC.Width(), rtDC.Height(), RGB(0xff, 0x0, 0xff));
	memDC.SelectObject(pOldBmp);
	return TRUE;
}

void CRadioButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	CString		strText;
	CDC			memDC;
	CBitmap		bitmap;
	CBitmap*	pOldBmp;
	CFont*		pOldFont;
	CRect		rect		= lpDrawItemStruct->rcItem;
	CRect		rtText		= rect;
	CRect		rtBmp		= rect;
	CPoint		ptBmp;

	ASSERT(ODT_BUTTON == lpDrawItemStruct->CtlType);

	if(FALSE == memDC.CreateCompatibleDC(pDC))
		return;
	if(FALSE == bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
		return;

	pOldBmp = memDC.SelectObject(&bitmap);
	pOldFont = memDC.SelectObject(GetFont());

	// 绘制背景
	DrawBk(&memDC);

	// 绘制按钮
	rtBmp.MoveToXY(0, (rtBmp.Height() - m_RadioBmpInfo.bmHeight) / 2);
	rtBmp.right = rtBmp.left + m_RadioBmpInfo.bmWidth / 2;
	rtBmp.bottom = rtBmp.top + m_RadioBmpInfo.bmHeight;
	ptBmp.y = 0;
	if((lpDrawItemStruct->itemState & ODS_CHECKED) == ODS_CHECKED)
	{
		ptBmp.x = m_RadioBmpInfo.bmWidth / 2;
	}
	else
	{
		ptBmp.x = 0;
	}
	DrawBitmap(&memDC, &m_RadioBitmap, rtBmp, ptBmp);

	// 绘制文本
	rtText.MoveToXY(0, 0);
	rtText.left += m_RadioBmpInfo.bmWidth / 2 + 5;
	GetWindowText(strText);
	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(strText, rtText, DT_SINGLELINE | DT_VCENTER);

	// 完成绘制
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 原还设备
	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldFont);
}

void CRadioButton::DrawBk(CDC* pDC)
{
	CWnd		*pParent;
	CRect		WndRect;
	POINT		OffPt;
	int			nSave;

	pParent = GetParent();

	GetWindowRect(&WndRect);
	GetParent()->ScreenToClient(&WndRect);

	OffPt.x = WndRect.left;
	OffPt.y = WndRect.top;

	nSave = pDC->SaveDC();

	pDC->SetWindowOrg(OffPt);
	pParent->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->m_hDC, 0);
	pDC->SetViewportOrg(OffPt);

	pDC->RestoreDC(nSave);
}

BOOL CRadioButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CRadioButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCheck(BST_CHECKED);

	CButton::OnLButtonDown(nFlags, point);
}
