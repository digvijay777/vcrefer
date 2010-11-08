#pragma once

#include "License2Doc.h"
// CLicense2View view

class CLicense2View : public CListView
{
	DECLARE_DYNCREATE(CLicense2View)

protected:
	CLicense2View();           // protected constructor used by dynamic creation
	virtual ~CLicense2View();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CMenu		m_menu;
private:
	VOID		UpdateContextMenu(DWORD dwMask);
public:
	CLicense2Doc*	GetDocument();
	LRESULT OnUpdateShow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDelete();
	afx_msg void OnModify();
	afx_msg void OnUpdateDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModify(CCmdUI *pCmdUI);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddlicensed();
//	afx_msg void OnPaint();
	afx_msg void OnScaninfo();
};


