#pragma once
#include <GdiPlus.h>

namespace gdip
{
	//////////////////////////////////////////////////////////////////////////
	// GDI+的内存DC
	template <class T>
	class TMemDC
	{
	public:
		TMemDC(HWND hWnd)
		{
			m_hWnd = hWnd;
			m_hDC = GetDC(m_hWnd);
			Get
		}
		TMemDC(HDC hDC, int nX, int nY, int nWidth, int nHeight)
		{
			m_hDC = hDC;
			m_rect.left = nX;
			m_rect.right = m_rect.left + nWidth;
			m_rect.top = nY;
			m_rect.bottom = m_rect.top + nHeight;
			m_bCancel = FALSE;
			m_hWnd = NULL;
			m_graphics = CreateMemDC(hDC, nWidth, nHeight);
		}
		~TMemDC()
		{
			if(NULL != m_graphics && FALSE == m_bCancel)
			{
				HDC		hMemDC		= m_graphics->GetHDC();

				::BitBlt(m_hDC, m_rect.left, m_rect.top, m_rect.right-m_rect.left, m_rect.bottom-m_rect.top
					, hMemDC, 0, 0, SRCCOPY);
				m_graphics->ReleaseHDC(hMemDC);
			}
			
			if(NULL != m_graphics)
			{
				HDC			hMemDC		= m_graphics->GetHDC();
				HBITMAP		hBitmap;

				delete m_graphics;
				m_graphics = NULL;
				hBitmap = SelectObject(hMemDC, m_hOldBitmap);
				DeleteObject((HOBJECT)hBitmap);
				DeleteObject((HOBJECT)hMemDC);
			}

			if(NULL != m_hWnd)
			{
				ReleaseDC(m_hWnd, m_hDC);
			}
		}
	public:
		HDC					m_hDC;
		HWND				m_hWnd;
		HBITMAP				m_hOldBitmap;
		RECT				m_rect;
		BOOL				m_bCancel;
		Gdiplus::Graphics*	m_graphics;

	public:
		// 创建内存DC
		HDC		CreateMemDC(HDC hDC, int nWidth, int nHeight)
		{
			HDC			hMemDC		= CreateCompatibleDC(hDC);
			HBITMAP		hBitmap		= CreateCompatibleBitmap(hDC, nWidth, nHeight);
			
			if(NULL == hMemDC)
				return hDC;

			m_hOldBitmap = SelectObject(hMemDC, (HOBJECT)hBitmap);
			return hMemDC;
		}
		// 取消内存DC
		void	Cancel()
		{
			m_bCancel = TRUE;
		}
	public:
		Gdiplus::Graphics* operator -> ()
		{
			return m_graphics;
		}
	};

}
