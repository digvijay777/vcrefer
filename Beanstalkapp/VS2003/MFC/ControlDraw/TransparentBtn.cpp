// TransparentBtn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "TransparentBtn.h"
#include ".\transparentbtn.h"


// CTransparentBtn

IMPLEMENT_DYNAMIC(CTransparentBtn, CButton)
CTransparentBtn::CTransparentBtn()
{
}

CTransparentBtn::~CTransparentBtn()
{
}


BEGIN_MESSAGE_MAP(CTransparentBtn, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CTransparentBtn ��Ϣ�������


BOOL CTransparentBtn::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	BOOL		bRet = TRUE;
// 	CRect		rtClient;
// 	CRect		rtSelf;
// 
// 	GetParent()->GetClientRect(&rtClient);
// 	GetWindowRect(&rtSelf);
// 	GetParent()->ScreenToClient(&rtSelf);
// 
// 	pDC->BitBlt(0, 0, rtSelf.Width(), rtSelf.Height(), GetParent()->GetDC(), rtSelf.left, rtSelf.top, SRCCOPY);
// 	//pDC->TextOut(0, 0, "bk");
// 	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

void CTransparentBtn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CButton::OnPaint()
	CString		szTitle;
	CRect		rect;

	static int i = 0; 
	GetWindowText(szTitle);
	GetClientRect(rect);
	if(i > rect.Width())
		i = 0;
	//rect.top -= i++;
	BOOL		bRet = TRUE;
	CRect		rtClient;
	CRect		rtSelf;

	GetParent()->GetClientRect(&rtClient);
	GetWindowRect(&rtSelf);
	GetParent()->ScreenToClient(&rtSelf);

	//dc.BitBlt(0, 0, rtSelf.Width(), rtSelf.Height(), GetParent()->GetDC(), rtSelf.left, rtSelf.top, SRCCOPY);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rtSelf.Width(), rtSelf.Height());
	CBitmap* pBmp = memDC.SelectObject(&bitmap);
	memDC.SetWindowOrg(rtSelf.left, rtSelf.top);
	GetParent()->SendMessage(WM_ERASEBKGND, (WPARAM)memDC.m_hDC, 0);
	memDC.SetViewportOrg(rtSelf.left, rtSelf.top);

	CRect text(rect);
	text.top += i++;
	dc.BitBlt(0, 0, rtSelf.Width(), rtSelf.Height(), &memDC, 0, 0, SRCCOPY);
	dc.DrawText(szTitle, &text, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	memDC.SelectObject(pBmp);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CTransparentBtn::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
