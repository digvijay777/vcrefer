#include "StdAfx.h"
#include "ImageDUIControl.h"
#include <assert.h>

#pragma comment(lib, "gdiplus.lib")

BOOL InitGdiplus()
{
	static ULONG_PTR		gdiToken		= NULL;

	if(NULL == gdiToken)
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiToken, &gdiplusStartupInput, NULL);
	}

	return (NULL != gdiToken);
};

Gdiplus::Image* ImageFromResouce(HINSTANCE hInstance, LPCTSTR lpResouceType, 
									 LPCTSTR lpResouceName)
{
	Gdiplus::Image*		pImage		= NULL;
	HRSRC				hRsrc;
	HGLOBAL				hResorce;
	DWORD				dwSize;
	CComPtr<IStream>	spStream;

	hRsrc = FindResource(hInstance, lpResouceName, lpResouceType);
	if(NULL == hRsrc)
	{
		return NULL;
	}

	hResorce = LoadResource(hInstance, hRsrc);
	if(NULL == hResorce)
	{
		return NULL;
	}
	// 构造Stream
	CreateStreamOnHGlobal(NULL, TRUE, &spStream);

	dwSize = SizeofResource(hInstance, hRsrc);
	spStream->Write(LockResource(hResorce), dwSize, &dwSize);
	// 创建图片
	pImage = Gdiplus::Image::FromStream(spStream);

	UnlockResource(hResorce);
	FreeResource(hResorce);

	return pImage;
}

//////////////////////////////////////////////////////////////////////////
CImageDUIRadio::CImageDUIRadio(Gdiplus::Image* pImage, CSimpleDUIBase* parent,
								 UINT uID, DUI_RADIO_GROUP* group)
: CSimpleDUIBase(parent)
, m_pImage(pImage)
, m_uID(uID)
, m_group(group)
{
	RECT		rect		= {0, 0, pImage->GetWidth() / 4, pImage->GetHeight()};

	MoveUI(&rect, FALSE);
	assert(NULL != pImage);
	assert(NULL != group);
	m_status = 0;
}

CImageDUIRadio::~CImageDUIRadio()
{

}

BOOL CImageDUIRadio::OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	RECT		rect;

	GetUIRect(&rect);
	if(WM_MOUSEMOVE == nMsg)
	{
		if(0 == m_status)
		{
			m_status = 1;
			UIInvalidate(&rect);
			TrackEvent(WM_MOUSELEAVE);
		}
	}
	else if(WM_MOUSELEAVE == nMsg)
	{
		m_status = 0;
		UIInvalidate(&rect);
	}
	else if(WM_LBUTTONDOWN == nMsg)
	{
		*m_group = this;
//		m_status = 1;
//		TrackEvent(WM_MOUSELEAVE);

		UIInvalidate(&rect);
		SendMessage(GetUIRoot()->hWnd, WM_COMMAND, m_uID, 0);
	}
	
	return TRUE;
}
/*
 *	绘制函数
 */
void CImageDUIRadio::OnUIDraw(HDC hDC, LPRECT lpRect)
{
	RECT					rect;
	int						nWidth		= m_pImage->GetWidth() / 4;
	int						nHeight		= m_pImage->GetHeight();
	int						nIndex		= 0;
	Gdiplus::Graphics		graphics(hDC);

	GetUIRect(&rect);
	MergerRect(&rect, &rect, lpRect);
	nIndex = ((this == *m_group)?1:0) * 2 + m_status % 2;
	graphics.DrawImage(m_pImage, 
		Gdiplus::Rect(rect.left, rect.top, nWidth, nHeight),
		nIndex * nWidth, 0, nWidth, nHeight, Gdiplus::UnitPixel);
}