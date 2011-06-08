#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <gdiplus.h>

template <class T>
class TGdipStatic : public CWindowImpl<TGdipStatic>
{
public:
	BEGIN_MSG_MAP(TGdipStatic)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

public:
	TGdipStatic()
	{
		memset(&m_LogFont, 0, sizeof(m_LogFont));
		m_color = Gdiplus::Color::MakeARGB(255, 0, 0, 0);
	}

protected:
	LOGFONTW			m_LogFont;
	Gdiplus::Color		m_color;

private:
	// 绘制
	virtual void DrawItem(HDC hDC)
	{
		Gdiplus::Graphics		graphic(hDC);
		Gdiplus::Font			font(hDC, &m_LogFont);
		Gdiplus::PointF			pt(0, 0);
		Gdiplus::SolidBrush		brush(m_color);

		graphic.DrawString(L"test", 4, &font, pt, &brush);
	}
	// 绘制消息
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		// 开始绘制
		PAINTSTRUCT				ps		= {0};
		HDC						hDC		= BeginPaint(&ps);

		if(0 == m_LogFont.lfHeight)
		{
			::GetObjectW((HGDIOBJ)SendMessage(WM_GETFONT), sizeof(LOGFONTW), &m_LogFont);
		}

		DrawItem(hDC);

		EndPaint(&ps);
		return 0;
	}

public:
	// 设置字体
	BOOL	SetFont(LPCWSTR lpFont, int nSize)
	{
		Gdiplus::Font		font(lpFont, nSize);
		Gdiplus::Graphics	graphics(m_hWnd);
		
		return Gdiplus::Ok == font.GetLogFontW(&graphics, &m_LogFont);
	}
	// 设置颜色
	BOOL	SetColor(BYTE a, BYTE r, BYTE g, BYTE b)
	{
		m_color = Gdiplus::Color::MakeARGB(a, r, g, b);
		return TRUE;
	}
};

typedef TGdipStatic<void>		CGdipStatic;
