#include "stdafx.h"
#include "TranspStatic.h"
#include ".\transpstatic.h"


IMPLEMENT_DYNAMIC(CTranspStatic, CStatic)
CTranspStatic::CTranspStatic()
{
	m_colTextColor = RGB(0x0, 0x0, 0x0);
}

CTranspStatic::~CTranspStatic()
{
}


BEGIN_MESSAGE_MAP(CTranspStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CTranspStatic message handlers
void CTranspStatic::SetTextColor(COLORREF col)
{
	m_colTextColor = col;
}

HBRUSH CTranspStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	CDC				memDC;
	CBitmap			bitmap;
	XRect			rect;
	CBitmap*		pObject;
	
	// ±³¾°
	if(NULL != m_brushBkgnd.GetSafeHandle())
		m_brushBkgnd.DeleteObject();

	GetParent()->GetClientRect(&rect);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(pDC);
	pObject = memDC.SelectObject(&bitmap);

	GetParent()->SendMessage(WM_ERASEBKGND, (WPARAM)memDC.GetSafeHdc());

	memDC.SelectObject(pObject);
	m_brushBkgnd.CreatePatternBrush(&bitmap);

	// ×ÖÌå
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colTextColor);

	return (HBRUSH)m_brushBkgnd.GetSafeHandle();
}
