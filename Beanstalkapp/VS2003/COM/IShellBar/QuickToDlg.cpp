// CQuickToDlg.cpp : Implementation of CCQuickToDlg

#include "stdafx.h"
#include "QuickToDlg.h"
#include ".\quicktodlg.h"


// CCQuickToDlg
HRESULT CQuickToDlg::GetWebBrowser()
{
	if(m_spWebBrowser != NULL)
		return S_OK;

	HRESULT							hres;
	CComPtr<IServiceProvider>		spClient;
	CComPtr<IServiceProvider>		spDocApp;
	LRESULT							lRet			= 1;

	hres = (*m_ppSite)->QueryInterface(&spClient);
	if(FAILED(hres))
		return lRet;

	hres = spClient->QueryService(SID_STopLevelBrowser, IID_IServiceProvider, (void **)&spDocApp);
	if(FAILED(hres))
		return lRet;

	hres = spDocApp->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (VOID **)&m_spWebBrowser);
	if(FAILED(hres))
		return lRet;

	return S_OK;
}

void CQuickToDlg::InitToolBar()
{
	wchar_t				szIniPath[MAX_PATH]		= {0};
	wchar_t*			pPoint					= NULL;

	GetModuleFileNameW((HMODULE)ghInstance, szIniPath, sizeof(szIniPath));
	pPoint = wcsrchr(szIniPath, L'.');
	if(NULL == pPoint)
		return;
	wcscpy(pPoint, L".ini");
	// 获取列表
	wchar_t			szList[1024]				= {0};
	wchar_t*		pName						= NULL;

	GetPrivateProfileStringW(L"QuickTo", NULL, L"", szList, sizeof(szList), szIniPath);
	for(pName = szList; 0 != pName[0]; pName += wcslen(pName) + 1)
	{
		wchar_t				szPath[MAX_PATH]	= {0};

		GetPrivateProfileStringW(L"QuickTo", pName, L"", szPath, sizeof(szPath), szIniPath);
		if(!PathFileExistsW(szPath))
			continue;
		// 添加记录
		BARITEMINFO			info;
		SHFILEINFOW			shell		= {0};

		SHGetFileInfoW(szPath, 0, &shell, sizeof(shell), SHGFI_ICON);
		info.sText = pName;
		info.sPath = szPath;
		info.hIcon = shell.hIcon;
		info.nState = 0;
		memset(&info.rt, 0, sizeof(info.rt));

		m_vctItem.push_back(info);
	}
}


LRESULT CQuickToDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for(int i = 0; i < m_vctItem.size(); i++)
	{
		if(NULL != m_vctItem[i].hIcon)
			DestroyIcon(m_vctItem[i].hIcon);
	}

	return CAxDialogImpl<CQuickToDlg>::OnDestroy(uMsg, wParam, lParam, bHandled);
}

LRESULT CQuickToDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT			PaintStruct		= {0};
	HDC					hDC;
	HDC					hMemDC;
	RECT				rt				= {0};
	RECT				rtMem			= {0};
	HBITMAP				hBitmap, hOldBmp;
	HFONT				hOldFont;

	hDC = BeginPaint(&PaintStruct);
	GetClientRect(&rt);
	rtMem.right = rt.right - rt.left;
	rtMem.bottom = rt.bottom - rt.top;

	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rt.right - rt.left, rt.bottom - rt.top);
	hOldBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);
	hOldFont = (HFONT)SelectObject(hMemDC, GetFont());

	DrawToolBar(hMemDC, rtMem);

	BitBlt(hDC, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top
		, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldFont);
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);

	EndPaint(&PaintStruct);
	return 0;
}

void CQuickToDlg::DrawToolBar(HDC hDC, RECT rt)
{
	// 背景
	::SetBkColor(hDC, GetSysColor(COLOR_MENU));
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
	// 绘制项
	RECT	rtDraw		= rt;
	RECT	rtItem;

	SetBkMode(hDC, TRANSPARENT);
	rtDraw.left += 3;
	for(int i = 0; i < m_vctItem.size(); i++)
	{
		BARITEMINFO&	bar			= m_vctItem[i];
		int				nHeight		= 0;

		memset(&rtItem, 0, sizeof(rtItem));
		// 计算大小
		rtItem = rtDraw;
		::DrawTextW(hDC, bar.sText.c_str(), bar.sText.size(), &rtItem, DT_CALCRECT);
		rtItem.right += 20;
		// 大小向后加
		rtDraw.left += rtItem.right - rtItem.left;
		// 绘制边框
		rtItem.top = rtDraw.top;
		rtItem.bottom = rtDraw.bottom;
		bar.rt = rtItem;	// 实际的位置
// 		InflateRect(&rtItem, 1, 2);
		if(1 == bar.nState)
		{
			DrawFrameControl(hDC, &rtItem, DFC_BUTTON, DFCS_BUTTONPUSH);
		}
		else if(2 == bar.nState)
		{
			DrawFrameControl(hDC, &rtItem, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
			rtItem.top += 1;
			rtItem.left += 1;
		}
// 		InflateRect(&rtItem, -1, -2);
		// 绘制图标和文本
		DrawIconEx(hDC, rtItem.left+1, rtItem.top + (rtItem.bottom - rtItem.top - 16) / 2, bar.hIcon, 16, 16, 1
			, GetSysColorBrush(COLOR_MENU), DI_IMAGE|DI_MASK);
		rtItem.left += 17;
		DrawTextW(hDC, bar.sText.c_str(), bar.sText.size(), &rtItem, DT_VCENTER | DT_SINGLELINE);
	}
}

LRESULT CQuickToDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(FALSE == m_bTrackMouse)
	{
		TRACKMOUSEEVENT		tmEvent		= {0};
		
		tmEvent.cbSize = sizeof(tmEvent);
		tmEvent.dwHoverTime = 1;
		tmEvent.hwndTrack = m_hWnd;
		tmEvent.dwFlags = TME_HOVER | TME_LEAVE;
		m_bTrackMouse = TrackMouseEvent(&tmEvent);
	}
	else
	{
		return OnMouseHover(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}

LRESULT CQuickToDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT		pt;

	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	m_nLBDItem = -1;
	for(int i = 0; i < (int)m_vctItem.size(); i++)
	{
		if(PtInRect(&m_vctItem[i].rt, pt))
		{
			m_nLBDItem = i;
			m_vctItem[i].nState = 2;
			SetCapture();
			InvalidateRect(&m_vctItem[i].rt);
		}
	}
	return 0;
}

LRESULT CQuickToDlg::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_nLBDItem > -1)
	{
		m_nLBDItem = -1;
		ReleaseCapture();
	}
	for(int i = 0; i < m_vctItem.size(); i++)
	{
		if(2 == m_vctItem[i].nState)
		{
			m_vctItem[i].nState = 1;
			InvalidateRect(&m_vctItem[i].rt);
			if(FAILED(GetWebBrowser()))
				return 0;
			m_spWebBrowser->Navigate((BSTR)m_vctItem[i].sPath.c_str(), &CComVariant(navNoWriteToCache), NULL, NULL, NULL);
			return 0;
		}
	}
	return 0;
}
LRESULT CQuickToDlg::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT		pt;

	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	for(int i = 0; i < (int)m_vctItem.size(); i++)
	{
		BOOL		bInvalid		= FALSE;

		if(PtInRect(&m_vctItem[i].rt, pt))
		{
			if(i == m_nLBDItem)
			{
				if(2 != m_vctItem[i].nState)
				{
					bInvalid = TRUE;
					m_vctItem[i].nState = 2;
				}
			}
			else if(m_nLBDItem > -1)
			{
				if(0 != m_vctItem[i].nState)
				{
					bInvalid = TRUE;
					m_vctItem[i].nState = 0;
				}
			}
			else
			{
				if(1 != m_vctItem[i].nState)
				{
					bInvalid = TRUE;
					m_vctItem[i].nState = 1;
				}
			}
		}
		else
		{
			if(0 != m_vctItem[i].nState)
			{
				bInvalid = TRUE;
				m_vctItem[i].nState = 0;
			}
		}

		if(bInvalid)
			InvalidateRect(&m_vctItem[i].rt);
	}
	ATLTRACE("******OnMouseHover*******: %08X (%d, %d)\n", GetTickCount(), pt.x, pt.y);
	return 0;
}

LRESULT CQuickToDlg::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for(int i = 0; i < (int)m_vctItem.size(); i++)
	{
		if(0 != m_vctItem[i].nState)
		{
			m_vctItem[i].nState = 0;
			InvalidateRect(&m_vctItem[i].rt);
		}
	}
	m_bTrackMouse = FALSE;
	return 0;
}
