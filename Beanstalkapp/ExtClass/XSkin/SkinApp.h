#pragma once
#include <map>
#include "SkinWnd.h"

class CSkinApp
{
public:
	CSkinApp(void);
	~CSkinApp(void);
public:
	BOOL		Install();
	BOOL		Uninstall();

protected:
	virtual BOOL	AttachWindow(HWND hWnd, CREATESTRUCT* pCreate);
	virtual BOOL	DetachWindow();

	BOOL			AttachSkinWindow(HWND hWnd, CSkinWnd* pSkinWnd);
	CSkinWnd*		GetSkinWnd(HWND hWnd);
	WNDPROC			GetOldWndProc(HWND hWnd);
private:
	typedef struct tagSkinWndData
	{
		WNDPROC		wndProc;
		CSkinWnd*	pSkinWnd;
		
		tagSkinWndData()
		{
			wndProc = NULL;
			pSkinWnd = NULL;
		}
	}SKINWNDDATA;
	typedef std::map<HWND, SKINWNDDATA>::iterator	SKINWNDDATAPTR;

	HHOOK		m_hhook;
	ULONG		m_pGdiToken;
	std::map<HWND, SKINWNDDATA>		m_mapWnds;
	
protected:
	void		ClearWndMap();
public:
	static CSkinApp*		s_pSkinApp;

	static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallSkinWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
