#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <vector>
#include <GdiPlus.h>
#include <string>
#include "../SimpleDUI.h"

#pragma comment(lib, "gdiplus.lib")

template <class T, BOOL AutoDelete = TRUE>
class TWndArrange
{
public:
	TWndArrange()
	{
		memset(&m_rect, 0, sizeof(m_rect));
		m_nHSpace = 0;
		m_nVSpace = 0;
		m_isVisible = TRUE;
	}

	~TWndArrange()
	{

	}

	typedef std::vector<T*>	WndList;

private:
	RECT		m_rect;
	int			m_nHSpace;		// ������
	int			m_nVSpace;		// ������
	BOOL		m_isVisible;
	WndList		m_wndList;

public:
	/*
	 *	���ò���
	 */
	void	Set(LPRECT lpRect, int nHSpace, int nVSpace)
	{
		m_rect = *lpRect;
		m_nHSpace = nHSpace;
		m_nVSpace = nVSpace;
	}
	/*
	 *	��մ���
	 */
	void	Clear()
	{
		for(size_t i; i < m_wndList.size(); i++)
		{
			if(NULL != m_wndList[i])
			{
				if( AutoDelete )
				{
					delete m_wndList[i];
				}
				m_wndList[i] = NULL;
			}
		}

		m_wndList.clear();
	}
	/*
	 *	�������д���
	 */
	void	ReArrange()
	{
		int			nTop		= m_rect.top;
		int			nLeft		= m_rect.left;
		int			nHeight		= 0;
		DWORD		dwShow		= (m_isVisible)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;

		dwShow |= SWP_NOSIZE | SWP_NOZORDER;
		for(size_t i; i < m_wndList.size(); i++)
		{
			RECT		rect		= {0};
			T*			pWnd		= m_wndList[i];

			if(NULL == pWnd)
			{
				continue;
			}
			
			pWnd->GetClientRect(&rect);
			if(m_rect.right < (rect.right + nLeft))
			{
				nTop += nHeight + m_nVSpace;
				nLeft = m_rect.left;
			}

			nHeight = max(nHeight, (rect.bottom - rect.top));
			pWnd->SetWindowPos(NULL, nTop, nLeft, 0, 0, dwShow);
			nLeft += rect.right - rect.left + m_nHSpace;
		}
	}
	/*
	 *  ��Ӵ���
	 */
	void	AddWnd(T* pWnd)
	{
		if(NULL == pWnd)
		{
			return;
		}
		
		m_wndList.push_back(pWnd);
		ReArrange();
	}
	/*
	 *	�ƶ�����
	 */
	BOOL	MoveWindow(LPRECT lpRect)
	{
		m_rect = *lpRect;
		ReArrange();
		return TRUE;
	}
	/*
	 *	��ʾ����
	 */
	void	ShowWindow(BOOL bVisible)
	{
		if(bVisible != m_isVisible)
		{
			m_isVisible = bVisible;
			ReArrange();
		}
	}
	/*
	 *	���д���ִ��һ��û�в����ĺ���
	 */
	template <class TFunction>
	void	Call(TFunction fn)
	{
		for(size_t i; i < m_wndList.size(); i++)
		{
			T*		pWnd		= m_wndList[i];

			if(NULL != pWnd)
			{
				pWnd->fn();
			}
		}
	}
	/*
	 *	���д���ִ�����������ĺ���
	 */
	template <class TFunction, typename TParam1>
	void	Call(TFunction fn, TParam1 pm1)
	{
		for(size_t i; i < m_wndList.size(); i++)
		{
			T*		pWnd		= m_wndList[i];

			if(NULL != pWnd)
			{
				pWnd->fn(pm1);
			}
		}
	}
};

/*
 *	�ؼ������Item��
 */
class CIconListItem : public CWindowImpl<CIconListItem>
{
private:
	CIconListItem(LPCWSTR lpText, HICON hIcon, Gdiplus::Image* pBk,
		Gdiplus::Image* pDel = NULL);
	~CIconListItem();

public:
	BEGIN_MSG_MAP(CIconListCtrl)
	END_MSG_MAP()

private:
	Gdiplus::Bitmap*	m_pIcon;
	Gdiplus::Image*		m_pBk;
	Gdiplus::Image*		m_pDel;
	WCHAR				m_szText[32];

public:
	CIconListItem*		CreateItem(HWND hParent, LPCWSTR lpText, HICON hIcon, 
		Gdiplus::Image* pBk, Gdiplus::Image* pDel = NULL);
};
/*
 *	������
 */
class CIconListNavigate : public CWindowImpl<CIconListNavigate>
{

};


/*
 *	�ؼ���
 */
class CIconListCtrl : public CWindowImpl<CIconListCtrl>,
	public CSimpleDUIPanel
{
public:
	CIconListCtrl();
	~CIconListCtrl();

public:
// 	BEGIN_MSG_MAP(CIconListCtrl)
// 		MESSAGE_HANDLER(WM_PAINT, OnPaint)
// 	END_MSG_MAP()
	CSimpleDUIText*		m_text;
	CSimpleDUIText*		m_text2;
	CSimpleDUIText*		m_text3;
	CSimpleDUIButton*	m_button;
	CSimpleDUIButton*	m_button2;

private:
//	TWndArrange<CIconListItem>		m_groups[5];
//	TWndArrange<CIconListNavigate>	m_navigate;
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);

public:
	BOOL	SubclassWindow(HWND hWnd);

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

