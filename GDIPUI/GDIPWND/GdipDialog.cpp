#include "stdafx.h"
#include "Gdip.h"
#include "GdipDialog.h"

CGdipDialog::CGdipDialog(HWND hWnd)
: CGdipWnd(hWnd)
{
	m_bkColor = 0xffffffff;
	m_hBkBrush = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
}

CGdipDialog::~CGdipDialog()
{
	if(NULL != m_hBkBrush)
		DeleteObject((HGDIOBJ)m_hBkBrush);
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
	case WM_CTLCOLORBTN:
		return TRUE;
	case WM_CTLCOLORSTATIC:
		*pResult = OnCtlColorStatic((HDC)wParam, (HWND)lParam);
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
	IStream*	pStream;
	HGLOBAL		hRes				= NULL;
	HGLOBAL		hGlobal				= NULL;

	hInstace = (HMODULE)GetWindowLong(m_hWnd, GWL_HINSTANCE);
	hSrc = FindResource(hInstace, lpID, lpType);
	if(NULL == hSrc)
	{
		ATLASSERT(NULL == hSrc);
		return -1;
	}
	dwSize = SizeofResource(hInstace, hSrc);
	hRes = LoadResource(hInstace, hSrc);
	hGlobal = GlobalAlloc(GMEM_FIXED, dwSize);

	BYTE*		pMem	= (BYTE*)GlobalLock(hGlobal);
	memcpy(pMem, hRes, dwSize);
	CreateStreamOnHGlobal(hGlobal, FALSE, &pStream);

	Bitmap*		pBmp	= Bitmap::FromStream(pStream);
	if(NULL != pBmp)
	{
		SetBkBitmap(pBmp);
		delete pBmp;
	}
	pStream->Release();
	FreeResource(hRes);
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);

	return 0;
}

LRESULT CGdipDialog::SetBk(LPCTSTR lpPath)
{
	Bitmap*		pBmp	= Bitmap::FromFile((WCHAR*)_bstr_t(lpPath));

	if(NULL != pBmp)
	{
		SetBkBitmap(pBmp);
		delete pBmp;
	}

	return 0;
}

// 设置背景
void CGdipDialog::SetBkBitmap(Bitmap* pBmp)
{
	// 设置窗口区域
	CreateRgnDlg(pBmp);

	// 设置背景BRUSH
	if(NULL != m_hBkBrush)
	{
		DeleteObject((HGDIOBJ)m_hBkBrush);
	}
	HBITMAP		hBitmap;
	pBmp->GetHBITMAP(m_bkColor, &hBitmap);
	m_hBkBrush = CreatePatternBrush(hBitmap);
}
// 画背景
LRESULT CGdipDialog::OnEraseBkGnd(HDC hDC)
{
	// 调用默认过程
	RECT		rtClient;

	GetClientRect(m_hWnd, &rtClient);
	FillRect(hDC, &rtClient, m_hBkBrush);

	return 0;
}

// 创建不规则的窗体
BOOL CGdipDialog::CreateRgnDlg(Bitmap* pBmp)
{
	if(NULL == pBmp)
		return FALSE;
	
	HRGN		hRgn				= ::CreateRectRgn(0, 0, 0, 0);
	Rect		rect(0, 0, 1, 1);

	for(int i = 0; i < pBmp->GetWidth(); i++)
	{
		for(int j = 0; j < pBmp->GetHeight(); j++)
		{
			Color		color;

			pBmp->GetPixel(i, j, &color);
			
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

// WM_CTLCOLORSTATIC
LRESULT CGdipDialog::OnCtlColorStatic(HDC hDC, HWND hWnd)
{
	::SetBkMode(hDC, TRANSPARENT);
	return (LRESULT)m_hBkBrush;
}
