#pragma once
#include <vector>

namespace NGdi
{
	typedef struct _FontText{
		HFONT		font;		// 字体
		COLORREF	color;		// 颜色
		RECT		rect;		// 区域
		WCHAR		wc;			// 字
		BOOL		bCreate;	// 是否创建新字体, 释放时需要删除字体
	} FONTTEXT;

	int			DrawRichText(HDC hDC, LPCWSTR lpText, int nLen, LPRECT lpRect, UINT uFormat);
	void		ParseFontText(LPCWSTR lpText, int nLen, HFONT hDef, COLORREF colDef, HDC hDC, std::vector<FONTTEXT>& vct);
	void		FreeFontText(std::vector<FONTTEXT>& vct);
	void		CalcFontTextRect(std::vector<FONTTEXT>& vct, HDC hDC);
}