#include "stdafx.h"
#include "Gdip.h"
#include "GdipDialog.h"

CGdipDialog::CGdipDialog(HWND hWnd)
: CGdipWnd(hWnd)
{
	m_pBkImage = NULL;	
	m_hGlobal = NULL;
	m_bkColor = 0xffffffff;
}

CGdipDialog::~CGdipDialog()
{
	if(NULL != m_hGlobal)
		GlobalFree(m_hGlobal);
}

// 窗口过程
BOOL CGdipDialog::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if(FALSE != CGdipWnd::WindowProc(Msg, wParam, lParam, pResult))
		return TRUE;

	switch(Msg)
	{
	case UM_GDIPDLG_SETBK:
		*pResult = SetBk((LPCTSTR)wParam);
		return TRUE;
	case UM_GDIPDLG_SETBK2:
		*pResult = SetBk((LPCSTR)wParam, (LPCSTR)lParam);
		return TRUE;
	case UM_GDIPDLG_SETBKCOLOR:
		*pResult = 1;
		m_bkColor = (ARGB)wParam;
		return TRUE;
	case WM_ERASEBKGND:
		*pResult = OnEraseBkGnd((HDC)wParam);
		return TRUE;
	case WM_NCHITTEST:
		*pResult = OnNcHisttest(LOWORD(lParam), HIWORD(lParam));
		return TRUE;
	}

	return FALSE;
}

// 设置背景
LRESULT CGdipDialog::SetBk(LPCTSTR lpID, LPCTSTR lpType)
{
	HRSRC		hSrc;
	DWORD		dwSize;
	HMODULE		hInstace;
	HGLOBAL		hGlobal;
	IStream*	pStream;

	hInstace = (HMODULE)GetWindowLong(m_hWnd, GWL_HINSTANCE);
	hSrc = FindResource(hInstace, lpID, lpType);
	if(NULL == hSrc)
	{
		ATLASSERT(NULL == hSrc);
		return -1;
	}
	dwSize = SizeofResource(hInstace, hSrc);

	if(NULL != m_hGlobal)
		GlobalFree(m_hGlobal);
	m_hGlobal	= GlobalAlloc(GMEM_FIXED, dwSize);
	BYTE*		pMem	= (BYTE*)GlobalLock(m_hGlobal);

	hGlobal = LoadResource(hInstace, hSrc);
	memcpy(pMem, hGlobal, dwSize);
	CreateStreamOnHGlobal(m_hGlobal, FALSE, &pStream);

	m_pBkImage = Bitmap::FromStream(pStream);

	GlobalUnlock(m_hGlobal);
	pStream->Release();
	FreeResource(hGlobal);

	CreateRgnDlg();
	return 0;
}

LRESULT CGdipDialog::SetBk(LPCTSTR lpPath)
{
	m_pBkImage = Bitmap::FromFile((WCHAR*)_bstr_t(lpPath));

	CreateRgnDlg();
	return 0;
}

// 画背景
LRESULT CGdipDialog::OnEraseBkGnd(HDC hDC)
{
	// 调用默认过程
	if(NULL == m_pBkImage)
	{
		return CGdipWnd::DefWindowProc(WM_ERASEBKGND, (WPARAM)hDC, NULL);
	}
	// 调用绘制过程
	RECT		rtClient;
	Graphics	graphics(hDC);

	graphics.Clear(Color(m_bkColor));
	GetClientRect(m_hWnd, &rtClient);
	Rect		rtDraw(rtClient.left, rtClient.top, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top);
	graphics.DrawImage(m_pBkImage,/* rtDraw,*/ 0, 0
		, m_pBkImage->GetWidth()
		, m_pBkImage->GetHeight()
		/*, UnitDisplay*/);

	return 0;
}

// 创建不规则的窗体
BOOL CGdipDialog::CreateRgnDlg()
{
	if(NULL == m_pBkImage)
		return FALSE;
	
	HRGN		hRgn				= ::CreateRectRgn(0, 0, 0, 0);
	Rect		rect(0, 0, 1, 1);

	for(int i = 0; i < m_pBkImage->GetWidth(); i++)
	{
		for(int j = 0; j < m_pBkImage->GetHeight(); j++)
		{
			Color		color;

			m_pBkImage->GetPixel(i, j, &color);
			
			if(0x0 != color.GetA())	// alpha 为0时不显示这块
			{
				HRGN		hPixelRgn	= ::CreateRectRgn(i, j, i+1, j+1);

				::CombineRgn(hRgn, hRgn, hPixelRgn, RGN_OR);
				DeleteObject((HGDIOBJ)hPixelRgn);
			}
		}
	}
	
	// 调整窗口大小
	RECT		rtWnd;

	GetRgnBox(hRgn, &rtWnd);
	SetWindowPos(m_hWnd, NULL, -1, -1, rtWnd.right - rtWnd.left
		, rtWnd.bottom - rtWnd.top, SWP_NOMOVE);
	// 去除标题栏
	DWORD		dwStyle		= GetWindowLong(m_hWnd, GWL_STYLE);

	dwStyle &= ~WS_CAPTION;
	dwStyle &= ~WS_BORDER;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	UpdateWindow(m_hWnd);
	// 设置区域
	SetWindowRgn(m_hWnd, hRgn, TRUE);
	DeleteObject((HGDIOBJ)hRgn);

	return TRUE;
}

// WM_NCHITTEST
LRESULT CGdipDialog::OnNcHisttest(int nX, int nY)
{
	return HTCAPTION;
}
