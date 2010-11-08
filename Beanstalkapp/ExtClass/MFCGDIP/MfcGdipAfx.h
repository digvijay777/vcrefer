/*
 *	MFCGDIP 公用头文件
 */
#pragma once

#ifndef _MFCGDIP_HEADER		// 避免重复定义
#define _MFCGDIP_HEADER

//////////////////////////////////////////////////////////////////////////
// 头文件申名
// math
#include <math.h>
// GDI+
#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>

using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////
// 消息定义
#define UM_BASE					(WM_USER+5120)

#define UM_DLGBASE				(UM_BASE+1)
#define UM_GETBKGNDBRUSH		UM_DLGBASE+1		// 得到背景的HBRUSH

//////////////////////////////////////////////////////////////////////////
// 公用函数申名

namespace MfcGdip
{
	// 透明背景
	BOOL DrawTranspBkgnd(HWND hWnd, HWND hParent, Graphics* const pGraphics = NULL);
	// 窗体坐标转换
	void ScreenToClient(HWND hWnd, LPRECT lpRect);
}

//////////////////////////////////////////////////////////////////////////
// 类申名

// GDI+配置内
class CGdipConfig
{
public:
	CGdipConfig();
	~CGdipConfig();
private:
	ULONG_PTR		m_pGdiToken;
};

extern CGdipConfig		gdipConfig;
// 通用RECT类
class XRect : public CRect
{
public:
	XRect();
	~XRect();
public:
	Gdiplus::Rect		Rect();
	Gdiplus::RectF		RectF();

	void operator=(Gdiplus::Rect& rt);
	void operator=(Gdiplus::RectF& rt);
};



#endif
