// CookiesLook.cpp : Implementation of CCookiesLook

#include "stdafx.h"
#include "CookiesLook.h"
#include "../../../ExtClass/XBase.h"

// CCookiesLook
STDMETHODIMP CCookiesLook::GetBandInfo(DWORD dwBandID,	DWORD dwViewMode, DESKBANDINFO* pdbi)
{
	if( NULL == pdbi )		return E_INVALIDARG;

	if(pdbi->dwMask & DBIM_MINSIZE)
	{
		RECT				rt			= {0};

		m_wndBar.GetWindowRect(&rt);
		pdbi->ptMinSize.x = rt.right - rt.left;
		pdbi->ptMinSize.y = rt.bottom - rt.top;
	}

	if(pdbi->dwMask & DBIM_MAXSIZE)
	{
		pdbi->ptMaxSize.x = -1;
		pdbi->ptMaxSize.y = -1;
	}

	if(pdbi->dwMask & DBIM_INTEGRAL)
	{
		pdbi->ptIntegral.x = 1;
		pdbi->ptIntegral.y = 1;
	}

	if(pdbi->dwMask & DBIM_ACTUAL)
	{
		pdbi->ptActual.x = 0;
		pdbi->ptActual.y = 0;
	}

	if(pdbi->dwMask & DBIM_TITLE)
	{
		//定制的浏览器栏窗口标题
		// wcscpy(pdbi->wszTitle,L"定制的工具栏");
	}

	if(pdbi->dwMask & DBIM_MODEFLAGS)
	{
		pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;
	}

	if(pdbi->dwMask & DBIM_BKCOLOR)
	{
		//如果使用默认的浏览器栏背景颜色，则移除本标志
		pdbi->dwMask &= ~DBIM_BKCOLOR;
	}
	return S_OK;
}

STDMETHODIMP CCookiesLook::ShowDW(BOOL bShow)
{
	m_wndBar.ShowWindow(bShow?SW_SHOW:SW_HIDE);
	return S_OK;
};

STDMETHODIMP CCookiesLook::CloseDW(DWORD dwReserved)
{
	m_wndBar.DestroyWindow();
	return S_OK;
};

STDMETHODIMP CCookiesLook::ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
	return E_NOTIMPL;
};

STDMETHODIMP CCookiesLook::ContextSensitiveHelp(BOOL fEnterMode)
{
	return S_OK;
}

STDMETHODIMP CCookiesLook::GetWindow(HWND * phwnd)
{
	*phwnd = m_wndBar.m_hWnd;
	return S_OK;
}

STDMETHODIMP CCookiesLook::SetSite(IUnknown *pUnkSite)
{
	if(NULL == pUnkSite)
	{
		// 释放操作	
	}
	else
	{
		HWND		hParentWnd		= NULL;
		// 加载Band的时候操作
		CComQIPtr<IOleWindow, &IID_IOleWindow>	spOleWnd(pUnkSite);

		if(spOleWnd)
			spOleWnd->GetWindow(&hParentWnd);
		if(NULL == hParentWnd)
			return E_FAIL;

		// 创建窗体
		m_wndBar.Create(hParentWnd);
	}

	return __super::SetSite(pUnkSite);
}