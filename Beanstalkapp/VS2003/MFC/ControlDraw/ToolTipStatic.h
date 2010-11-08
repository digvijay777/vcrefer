#pragma once


// CToolTipStatic

class CToolTipStatic : public CStatic
{
	DECLARE_DYNAMIC(CToolTipStatic)

public:
	CToolTipStatic();
	virtual ~CToolTipStatic();

protected:
	DECLARE_MESSAGE_MAP()

	CToolTipCtrl		m_tooltip;
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


