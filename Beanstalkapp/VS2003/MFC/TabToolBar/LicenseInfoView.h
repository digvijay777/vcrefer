#pragma once

#include "LicenseInfoDoc.h"
// CLicenseInfoView view

class CLicenseInfoView : public CListView
{
	DECLARE_DYNCREATE(CLicenseInfoView)

protected:
	CLicenseInfoView();           // protected constructor used by dynamic creation
	virtual ~CLicenseInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateList(WPARAM wParam, LPARAM lParam);
public:
	CLicenseInfoDoc*	GetDocument();
	afx_msg void OnTbSearch();
	afx_msg void OnModify();
	afx_msg void OnUpdateModify(CCmdUI *pCmdUI);
};


