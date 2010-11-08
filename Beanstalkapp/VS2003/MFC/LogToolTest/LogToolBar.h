#pragma once


// CLogToolBar

class CLogToolBar : public CDialogBar
{
	DECLARE_DYNAMIC(CLogToolBar)

public:
	CLogToolBar();
	virtual ~CLogToolBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nStyle, CSize size, UINT nID);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};


