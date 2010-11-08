#pragma once


// CToolView

class CToolView : public CReBar
{
	DECLARE_DYNAMIC(CToolView)

public:
	CToolView();
	virtual ~CToolView();

private:
	DWORD		m_dwDockStyle;
	DWORD		m_dwStyle;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd, CRuntimeClass* pView, CSize size, UINT nID);
};


