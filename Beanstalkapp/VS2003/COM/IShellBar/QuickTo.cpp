// QuickTo.cpp : Implementation of CQuickTo

#include "stdafx.h"
#include "QuickTo.h"


// CQuickTo
STDMETHODIMP CQuickTo::GetBandInfo(DWORD dwBandID,	DWORD dwViewMode, DESKBANDINFO* pdbi)
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
		//���Ƶ�����������ڱ���
		wcscpy(pdbi->wszTitle, L"���ٵ���");
	}

	if(pdbi->dwMask & DBIM_MODEFLAGS)
	{
		pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;
	}

	if(pdbi->dwMask & DBIM_BKCOLOR)
	{
		//���ʹ��Ĭ�ϵ��������������ɫ�����Ƴ�����־
		pdbi->dwMask &= ~DBIM_BKCOLOR;
	}
	return S_OK;
}

STDMETHODIMP CQuickTo::ShowDW(BOOL bShow)
{
	m_wndBar.ShowWindow(bShow?SW_SHOW:SW_HIDE);
	return S_OK;
};

STDMETHODIMP CQuickTo::CloseDW(DWORD dwReserved)
{
	m_wndBar.DestroyWindow();
	return S_OK;
};

STDMETHODIMP CQuickTo::ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
	return E_NOTIMPL;
};

STDMETHODIMP CQuickTo::ContextSensitiveHelp(BOOL fEnterMode)
{
	return S_OK;
}

STDMETHODIMP CQuickTo::GetWindow(HWND * phwnd)
{
	*phwnd = m_wndBar.m_hWnd;
	return S_OK;
}

STDMETHODIMP CQuickTo::SetSite(IUnknown *pUnkSite)
{
	if(NULL == pUnkSite)
	{
		// �ͷŲ���	
	}
	else
	{
		HWND		hParentWnd		= NULL;

		// ����Band��ʱ�����
		CComQIPtr<IOleWindow, &IID_IOleWindow>	spOleWnd(pUnkSite);

		if(spOleWnd)
			spOleWnd->GetWindow(&hParentWnd);
		if(NULL == hParentWnd)
			return E_FAIL;

		// ��������
		m_wndBar.Create(hParentWnd, (LPARAM)&m_spUnkSite);
	}

	return __super::SetSite(pUnkSite);
}
