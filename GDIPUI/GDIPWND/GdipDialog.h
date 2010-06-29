#pragma once
#include "GdipWnd.h"

class CGdipDialog : public CGdipWnd
{
public:
	CGdipDialog(HWND hWnd);
	~CGdipDialog();

public:
	virtual BOOL		WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

private:
	Bitmap*		m_pBkImage;
	HGLOBAL		m_hGlobal;
	ARGB		m_bkColor;

public:
	virtual LRESULT		SetBk(LPCTSTR lpPath);
	virtual LRESULT		SetBk(LPCTSTR lpID, LPCTSTR lpType);
	virtual LRESULT		OnEraseBkGnd(HDC hDC);
	virtual BOOL		CreateRgnDlg();
	virtual LRESULT		OnNcHisttest(int nX, int nY);
};