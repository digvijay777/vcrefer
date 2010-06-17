#include "stdafx.h"
#include "XCaptionButton.h"

#ifndef TRANSPARENT_BKCOLOR
	#define TRANSPARENT_BKCOLOR			RGB(0xFF, 0, 0xFF)
#endif

#if defined(ATLASSERT) && !defined(ASSERT)
#define ASSERT		ATLASSERT
#elif !defined(ASSERT)
#define ASSERT		assert
#endif

std::vector<CXCaptionButton *>		CXCaptionButton::s_vctXCaptionButton;

CXCaptionButton::CXCaptionButton()
{

}

CXCaptionButton::~CXCaptionButton()
{
	if(NULL != m_btImg.m_hObject)
		m_btImg.DeleteObject();
}

/*
 *	������������ť
 */
BOOL CXCaptionButton::CreateSysButton(UINT uiBk, POINT pt, UINT ID)
{
	ASSERT(NULL == m_btImg.m_hObject);

	// ����λͼ
	if(FALSE == m_btImg.LoadBitmap(uiBk))
	{
		return FALSE;
	}

	// ����λͼ
	BITMAP		bitmap;

	m_btImg.GetBitmap(&bitmap);
	m_rect.SetRect(pt.x, pt.y, pt.x + bitmap.bmWidth / 4, pt.y + bitmap.bmHeight);
	m_uID = ID;

	// ���ô��ڹ���
	
}