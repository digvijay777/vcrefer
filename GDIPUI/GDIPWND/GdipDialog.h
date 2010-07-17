#pragma once
#include "GdipWnd.h"

class CGdipDialog : public CGdipWnd
{
public:
	CGdipDialog(HWND hWnd);
	virtual ~CGdipDialog();

public:
	virtual BOOL		WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

private:
	ARGB		m_bkColor;
	HBRUSH		m_hBkBrush;

	void		SetBkBitmap(Bitmap* pBmp);
public:
	virtual LRESULT		SetBk(LPCTSTR lpPath);
	virtual LRESULT		SetBk(LPCTSTR lpID, LPCTSTR lpType);
	virtual LRESULT		OnEraseBkGnd(HDC hDC);
	virtual BOOL		CreateRgnDlg(Bitmap* pBmp);
	virtual LRESULT		OnNcHisttest(int nX, int nY);
	virtual LRESULT		OnCtlColorStatic(HDC hDC, HWND hWnd);
};