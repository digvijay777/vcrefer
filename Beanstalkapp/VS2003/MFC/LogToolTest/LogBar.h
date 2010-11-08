#pragma once


// CLogBar

class CLogBar : public CReBar
{
	DECLARE_DYNAMIC(CLogBar)

public:
	CLogBar();
	virtual ~CLogBar();

protected:
	DECLARE_MESSAGE_MAP()

	CDialogBar		m_dlgBar;
	CStatic			m_ctlText;
	CWnd			m_ctlBasebar;
public:
	virtual BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID);
	void EnableDocking(DWORD dwDockStyle);
};


