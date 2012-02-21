#pragma once
#include <vector>

namespace NGdi
{
	typedef struct _FontText{
		HFONT		font;		// ����
		COLORREF	color;		// ��ɫ
		RECT		rect;		// ����
		WCHAR		wc;			// ��
		BOOL		bCreate;	// �Ƿ񴴽�������, �ͷ�ʱ��Ҫɾ������
	} FONTTEXT;

	int			DrawRichText(HDC hDC, LPCWSTR lpText, int nLen, LPRECT lpRect, UINT uFormat);
	void		ParseFontText(LPCWSTR lpText, int nLen, HFONT hDef, COLORREF colDef, HDC hDC, std::vector<FONTTEXT>& vct);
	void		FreeFontText(std::vector<FONTTEXT>& vct);
	void		CalcFontTextRect(std::vector<FONTTEXT>& vct, HDC hDC);
}