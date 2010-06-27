#include "stdafx.h"
#include "Gdip.h"
#include "GdipDialog.h"

CGdipDialog::CGdipDialog(HWND hWnd)
: CGdipWnd(hWnd)
{
	m_pBkImage = NULL;	
	m_hGlobal = NULL;
}

CGdipDialog::~CGdipDialog()
{
	if(NULL != m_hGlobal)
		GlobalFree(m_hGlobal);
}

// 窗口过程
BOOL CGdipDialog::WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch(Msg)
	{
	case UM_GDIPDLG_SETBK:
		*pResult = SetBk((LPCTSTR)wParam);
		return TRUE;
	case UM_GDIPDLG_SETBK2:
		*pResult = SetBk((LPCSTR)wParam, (LPCSTR)lParam);
		return TRUE;
	case WM_ERASEBKGND:
		*pResult = OnEraseBkGnd(Graphics::FromHDC((HDC)wParam));
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

	m_pBkImage = Image::FromStream(pStream);

	GlobalUnlock(m_hGlobal);
	pStream->Release();
	FreeResource(hGlobal);
	return 0;
}

LRESULT CGdipDialog::SetBk(LPCTSTR lpPath)
{
	m_pBkImage = Image::FromFile((WCHAR*)_bstr_t(lpPath));

	return 0;
}

// 画背景
LRESULT CGdipDialog::OnEraseBkGnd(Graphics* pGraphics)
{
	// 调用默认过程
	if(NULL == pGraphics || NULL == m_pBkImage)
	{
		return CGdipWnd::DefWindowProc(WM_ERASEBKGND, (WPARAM)pGraphics->GetHDC(), NULL);
	}
	// 调用绘制过程
	RECT		rtClient;

	GetClientRect(m_hWnd, &rtClient);
	Rect		rtDraw(rtClient.left, rtClient.top, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top);
	pGraphics->DrawImage(m_pBkImage,/* rtDraw,*/ 0, 0
		, rtDraw.Width
		, rtDraw.Height
		/*, UnitDisplay*/);

	return 0;
}