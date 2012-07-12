#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <vector>
#include <GdiPlus.h>
#include <string>
#include "../SimpleDUI.h"

#pragma comment(lib, "gdiplus.lib")

/*
 *	项
 */
class CILItem : CSimpleDUIBase
{
public:
	CILItem(CSimpleDUIBase* parent);
	virtual ~CILItem();
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
	std::vector<CSimpleDUIBase*>	m_navigates;
	size_t							m_nShowGroup;

public:
	virtual void		OnUIDraw(HDC hDC, LPRECT lpRect);
	virtual BOOL		OnUIEvent(UINT nMsg, WPARAM wParam, LPARAM lParam);

public:
	BOOL		AddGroup();
	BOOL		DeleteGroup(int nIndex);
	BOOL		AddItem(int nGroup, CILItem* item);
	BOOL		DeleteItem(int nGroup, CILItem* item);

public:
	BOOL		ShowGroup(int nIndex);
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
// 	BEGIN_MSG_MAP(CIconListCtrl)
// 		MESSAGE_HANDLER(WM_PAINT, OnPaint)
// 	END_MSG_MAP()

// 	CSimpleDUIText*		m_text;
// 	CSimpleDUIText*		m_text2;
// 	CSimpleDUIText*		m_text3;
// 	CSimpleDUIButton*	m_button;
// 	CSimpleDUIButton*	m_button2;

private:
//	TWndArrange<CIconListItem>		m_groups[5];
//	TWndArrange<CIconListNavigate>	m_navigate;
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);

public:
	BOOL	SubclassWindow(HWND hWnd);

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

