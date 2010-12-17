#pragma once


// CHistoryListView view

class CTaskListView : public CListView
{
	DECLARE_DYNCREATE(CTaskListView)

protected:
	CTaskListView();           // protected constructor used by dynamic creation
	virtual ~CTaskListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


