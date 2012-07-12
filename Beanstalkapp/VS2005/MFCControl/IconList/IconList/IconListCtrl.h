#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <vector>
#include <GdiPlus.h>
#include <string>
#include "../SimpleDUI.h"

#pragma comment(lib, "gdiplus.lib")

/*
 *	��
 */
class CILItem : CSimpleDUIBase
{
public:
	CILItem(CSimpleDUIBase* parent);
	virtual ~CILItem();
};

/*
 *	�������
 */
class CILContainer : public CSimpleDUIBase
{
public:
	CILContainer(CSimpleDUIBase* parent);
	virtual ~CILContainer();

public:
	std::vector<CSimpleDUIBase*>	m_groups;
	CSimpleDUIBase*					m_navigatebar;
	std::vector<CSimpleDUIBase*>	m_navigates;

public:
	virtual void				OnDraw(HDC hDC, LPRECT lpRect);
};
/*
 *	�ؼ���
 */
class CIconListCtrl : public CWindowImpl<CIconListCtrl>,
	public CSimpleDUIRoot
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

	CILContainer*		m_container;

private:
//	TWndArrange<CIconListItem>		m_groups[5];
//	TWndArrange<CIconListNavigate>	m_navigate;
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);

public:
	BOOL	SubclassWindow(HWND hWnd);

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

