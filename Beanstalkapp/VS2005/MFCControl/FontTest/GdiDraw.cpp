#include "StdAfx.h"
#include "GdiDraw.h"

#ifndef FlagOn
#define FlagOn(_F, _SF)		( ((_F) & (_SF)) == (_F) )
#endif

/*
 *	�ḻ���ı����ƹ���
 *  ����: ͬAPI:DrawText
 *  �ı����ո�ʽ: a. <font [color='#RGB'] [size=''] [family=''] [italic='0/1'] [line='under/delete/dual'] [bold='0/1']></font>
 */
int NGdi::DrawRichText(HDC hDC, LPCWSTR lpText, int nLen, LPRECT lpRect, UINT uFormat)
{
	int							nSaveDC			= SaveDC(hDC);
	HFONT						hDef			= (HFONT)SelectObject(hDC, (HFONT)GetStockObject(DEFAULT_GUI_FONT));
	COLORREF					colDef			= GetTextColor(hDC);
	std::vector<FONTTEXT>		vctFontText;

	// ��ʼ����������
	ParseFontText(lpText, nLen, hDef, colDef, hDC, vctFontText);
	if(0 == (int)vctFontText.size())
		return 0;
	// ��ʼ����
	if(FlagOn(DT_SINGLELINE, uFormat))
	{
		RECT		rect		= *lpRect;
		int			i			= 0;
		
		do
		{
// 			if()
// 			i++;
		} where(i < (int)vctFontText.size())
	}
	// �ͷŲ���
	FreeFontText(vctFontText);
	SelectObject(hDC, hDef);
	RestoreDC(hDC, nSaveDC);
	return 0;
}

/*
 *	��������
 */
void NGdi::ParseFontText(LPCWSTR lpText, int nLen, HFONT hDef, COLORREF colDef, HDC hDC, std::vector<FONTTEXT>& vct)
{
	HFONT		hFontLast		= hDef;
	COLORREF	colLast			= colDef;

	for(int i = 0; i < nLen; i++)
	{
		FONTTEXT		ft		= {0};

		ft.color = colDef;
		ft.font = hFontLast;
		ft.bCreate = FALSE;
		ft.wc = lpText[i];

		if(L'<' == lpText[i])
		{
			// ����������ʽ
			if(L'<' == lpText[i+1])
			{
				i++;
			}
			else if(L'/' == lpText[i+1])
			{
				LPCWSTR				pPos	= NULL;

				hFontLast = hDef;
				colLast = colDef;
				pPos = wcschr(lpText+i, '>');
				if(NULL == pPos)
					break; // ����������
				i = (int)(__int64)(pPos - lpText);
			}
			else
			{
				// ָ�����
				LPCWSTR				pPos			= NULL;
				WCHAR				szFont[512]		= {0};
				
				pPos = wcschr(lpText+i, '>');
				if(NULL == pPos)
					break; // ����������
				wcsncpy(szFont, lpText+i+1, min(511, (int)(__int64)(pPos - lpText) - i));
				i = (int)(__int64)(pPos - lpText);
				
				if(wcsncmp(szFont, L"font ", 5) != 0)
					continue; // ���������µı��
				// ��ʼƥ������
				LOGFONTW		lfOld			= {0};
				LOGFONTW		lfNew			= {0};
				LPCWSTR			pStyle			= NULL;

				// ����������
				::GetObject(hDef, sizeof(LOGFONTW), &lfOld);
				memcpy(&lfNew, &lfOld, sizeof(LOGFONTW));
				// ��ɫ
				pStyle = wcsstr(szFont, L"color='#");
				if(NULL != pStyle)
				{
					LONG nCol = colDef;
					swscanf(pStyle + 8, L"%x", &nCol);
					colLast = RGB( (0xff0000 & nCol) >> 16, (0xff00 & nCol) >> 8, 0xff & nCol );
				}
				// �ֺ�
				pStyle = wcsstr(szFont, L"size='");
				if(NULL != pStyle)
				{
					lfNew.lfHeight = -MulDiv(_wtoi(pStyle+6), GetDeviceCaps(hDC, LOGPIXELSY), 72);
				}
				// ����
				pStyle = wcsstr(szFont, L"family='");
				if(NULL != pStyle)
				{
					wcsncpy(lfNew.lfFaceName, pStyle, 31);
					WCHAR* pEnd = (WCHAR*)wcschr(lfNew.lfFaceName, L'\'');
					if(NULL == pStyle) 
						pEnd = (WCHAR*)wcschr(lfNew.lfFaceName, L'\x20');
					if(NULL == pStyle)
						pEnd = (WCHAR*)wcschr(lfNew.lfFaceName, L'>');
					if(NULL == pEnd)
						continue;
					*pEnd = 0;
				}
				// б��
				pStyle = wcsstr(szFont, L"italic='");
				if(NULL != pStyle)
				{
					lfNew.lfItalic = _wtoi(pStyle+8);
				}
				// Line
				pStyle = wcsstr(szFont, L"line='");
				if(NULL != pStyle)
				{
					WCHAR		szLine[32]		= {0};
					WCHAR*		pS				= NULL;
					
					wcsncpy(szLine, pStyle+6, 31);
					pS = (WCHAR*)wcschr(szLine, L'\'');
					WCHAR* pEnd = (WCHAR*)wcschr(szLine, L'\'');
					if(NULL == pStyle) 
						pEnd = (WCHAR*)wcschr(szLine, L'\x20');
					if(NULL == pStyle)
						pEnd = (WCHAR*)wcschr(szLine, L'>');
					if(NULL == pEnd)
						continue;
					*pEnd = 0;
					if(wcscmp(L"under", szLine) == 0)
						lfNew.lfUnderline = 1;
				}
				// ����������
				if(memcpy(&lfNew, &lfOld, sizeof(lfNew)) != 0)
				{
					hFontLast = CreateFontIndirectW(&lfNew);
					ft.font = hFontLast;
					ft.color = colLast;
					ft.bCreate = TRUE;
					ft.wc = lpText[++i];
				}
			}
		}

		vct.push_back(ft);
	}
}
/*
 *	�ͷ�����
 */
void NGdi::FreeFontText(std::vector<FONTTEXT>& vct)
{
	for(int i = 0; i < (int)vct.size(); i++)
	{
		if(vct[i].bCreate)
			DeleteObject(vct[i].font);
	}
}