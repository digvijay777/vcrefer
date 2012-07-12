#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <vector>
#include <GdiPlus.h>
#include <string>
#include "../SimpleDUI.h"
#include "../ImageDUIControl.h"
/*
 *	项
 */
class CILItem : CSimpleDUIBase
{
public:
	CILItem(Gdiplus::Image* pImage, CSimpleDUIBase* parent, HICON hIcon,
		LPCWSTR lpText, UINT uID);
	virtual ~CILItem();

public:
	Gdiplus::Bitmap*	m_icon;
	Gdiplus::Image*		m_image;
	WCHAR				m_szText[33];
	UINT				m_uID;
	int					m_status;

public:
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
};

/*
 *	划动面板
 */
class CILContainer : public CSimpleDUIRoot
{
public:
	CILContainer();
	virtual ~CILContainer();

public:
	std::vector<CSimpleDUIBase*>	m_groups;
	CSimpleDUIBase*					m_navigatebar;
	size_t							m_nCurrentGroup;
	size_t							m_nSwitchGroup;
	std::vector<CSimpleDUIBase*>	m_navigates;
	Gdiplus::Image*					m_imageRadio;
	Gdiplus::Image*					m_imageItemBk;
	DUI_RADIO_GROUP					m_radiogroup;

public:
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);

public:
	BOOL		TranslateUIEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	BOOL		AddGroup();
	BOOL		DeleteGroup(int nIndex);
	BOOL		AddItem(int nGroup, HICON hIcon, LPCWSTR lpText, UINT uID);
	BOOL		DeleteItem(int nGroup, UINT uID);
	BOOL		UpdateNaviageBar();
	BOOL		UpdateGroup(int nGroup);

public:
	BOOL		ShowGroup(int nIndex);
	BOOL		AnimationShowGroup(int nIndex);
};
/*
 *	控件类
 */
class CIconListCtrl : public CWindowImpl<CIconListCtrl>,
	public CILContainer
{
public:
	CIconListCtrl();
	~CIconListCtrl();

public:
// 	CSimpleDUIText*		m_text;
// 	CSimpleDUIText*		m_text2;
// 	CSimpleDUIText*		m_text3;
// 	CSimpleDUIButton*	m_button;
// 	CSimpleDUIButton*	m_button2;

private:
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);

public:
	BOOL	SubclassWindow(HWND hWnd);
};

