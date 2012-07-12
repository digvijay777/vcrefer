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
class CILItem : public CSimpleDUIBase
{
public:
	CILItem(Gdiplus::Image* pImage, Gdiplus::Image* pImageEt, 
		CSimpleDUIBase* parent, HICON hIcon, LPCWSTR lpText, UINT uID);
	virtual ~CILItem();

public:
	Gdiplus::Bitmap*	m_icon;
	Gdiplus::Image*		m_image;
	WCHAR				m_szText[33];
	UINT				m_uID;
	int					m_status;
	CImageDUIButton*	m_et;

public:
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);

public:
	void		SetEditMode(BOOL bEtMode);
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
	Gdiplus::Image*					m_imageItemEt;
	DUI_RADIO_GROUP					m_radiogroup;
	// 通知主界面有事件发生 wParam, 消息ID, lParam, 高位指定组，低位指定模式0正常，1编辑
	UINT							m_uNotifyMsg;		
	HWND							m_hNotifyWnd;

public:
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);

public:
	BOOL		TranslateUIEvent(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	void		SetGroupCount(size_t nCount);
	size_t		GetGroupCount();
	BOOL		AddItem(int nGroup, HICON hIcon, LPCWSTR lpText, UINT uID);
	BOOL		DeleteItem(int nGroup, UINT uID);
	BOOL		UpdateNaviageBar();
	BOOL		UpdateGroup(int nGroup);
	void		SetEditMode(BOOL bEtMode);

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

private:
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);

public:
	BOOL	SubclassWindow(HWND hWnd);
};

