#pragma once
#include "SimpleDUI.h"
#include <GdiPlus.h>

// 初始化GDI+
BOOL				InitGdiplus();
Gdiplus::Image*		ImageFromResouce(HINSTANCE hInstance, LPCTSTR lpResouceType,
									 LPCTSTR lpResouceName);

typedef CSimpleDUIBase*		DUI_RADIO_GROUP;
/*
 *	 radio组件
 */
class CImageDUIRadio : public CSimpleDUIBase
{
public:
	CImageDUIRadio(Gdiplus::Image* pImage, CSimpleDUIBase* parent, 
		UINT uID, DUI_RADIO_GROUP* group);
	virtual ~CImageDUIRadio();

public:

public:
	Gdiplus::Image*		m_pImage;
	UINT				m_uID;
	int					m_status;
	DUI_RADIO_GROUP		*m_group;

public:
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
};
/*
 *	 Button
 */
class CImageDUIButton : public CSimpleDUIBase
{
public:
	CImageDUIButton(Gdiplus::Image* pImage, CSimpleDUIBase* parent, UINT uID);
	virtual ~CImageDUIButton();

public:
	Gdiplus::Image*		m_image;
	int					m_status;
	UINT				m_uID;

public:
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
};
