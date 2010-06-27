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
	Image*		m_pBkImage;
	HGLOBAL		m_hGlobal;

public:
	virtual LRESULT		SetBk(LPCTSTR lpPath);
	virtual LRESULT		SetBk(LPCTSTR lpID, LPCTSTR lpType);
	virtual LRESULT		OnEraseBkGnd(Graphics* pGraphics);
};