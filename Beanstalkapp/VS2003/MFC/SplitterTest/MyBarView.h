#pragma once
#include "BarView.h"

// CMyBarView view

class CMyBarView : public CBarView
{
	DECLARE_DYNCREATE(CMyBarView)

protected:
	CMyBarView();           // protected constructor used by dynamic creation
	virtual ~CMyBarView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


