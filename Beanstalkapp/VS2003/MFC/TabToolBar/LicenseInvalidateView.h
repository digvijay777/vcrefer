#pragma once
#include "LicenseInvalidateDoc.h"

// CLicenseInvalidateView view

class CLicenseInvalidateView : public CListView
{
	DECLARE_DYNCREATE(CLicenseInvalidateView)

protected:
	CLicenseInvalidateView();           // protected constructor used by dynamic creation
	virtual ~CLicenseInvalidateView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	CMenu		m_menu;
protected:
	DECLARE_MESSAGE_MAP()
public:
	CLicenseInvalidateDoc*	GetDocument();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
private:
	VOID		UpdateContextMenu(DWORD dwMask);
public:
	afx_msg void OnScaninfo();
};


