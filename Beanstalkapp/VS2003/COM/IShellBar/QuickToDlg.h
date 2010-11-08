// CQuickToDlg.h : Declaration of the CQuickToDlg

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include <shlobj.h>
#include <vector>
#include <string>

#define WM_GETISHELLBROWSER				(WM_USER+7)
// CQuickToDlg
class CQuickToDlg : 
	public CAxDialogImpl<CQuickToDlg>
{
public:
	CQuickToDlg()
	{
		m_nLBDItem = -1;
		m_bTrackMouse = FALSE;
	}

	~CQuickToDlg()
	{
	}

	enum { IDD = IDD_CQUICKTODLG };

BEGIN_MSG_MAP(CQuickToDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
	MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
	CHAIN_MSG_MAP(CAxDialogImpl<CQuickToDlg>)
END_MSG_MAP()

	typedef struct tagBarItemInfo{
		std::wstring	sText;
		std::wstring	sPath;
		HICON			hIcon;
		RECT			rt;
		int				nState;
	}BARITEMINFO;
private:
	IUnknown**					m_ppSite;
	CComPtr<IWebBrowser2>		m_spWebBrowser;
	std::vector<BARITEMINFO>	m_vctItem;
	int							m_nLBDItem;
	BOOL						m_bTrackMouse;

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CQuickToDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		
		m_ppSite = (IUnknown **)lParam;

		InitToolBar();
		return 1;  // Let the system set the focus
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT				GetWebBrowser();
	void				InitToolBar();
	
	virtual void		DrawToolBar(HDC hDC, RECT rt);
};


