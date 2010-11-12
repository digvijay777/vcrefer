#pragma once
#include <vector>
#include <string>

// CBarView view

class CBarView : public CView
{
	DECLARE_DYNCREATE(CBarView)

protected:
	CBarView();           // protected constructor used by dynamic creation
	virtual ~CBarView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	typedef struct tagBarViewInfo{
		CWnd*			pWnd;
		int				nHeight;
		std::string		strTtile;
		BOOL			bExpend;
		int				nMaxHeight;
	}BARVIEWINFO;

	enum ID{ID_MAINVIEW = 1, ID_BARVIEW = 10};

	HCURSOR		m_hSizeCursor;
	HCURSOR		m_hDefCursor;
	BOOL		m_bTracking;

	CRect		m_rectTracker;
	CRect		m_rectLimt;
	int			m_nTrackerBar;
protected:
	CWnd*						m_pMainView;
	std::vector<BARVIEWINFO>	m_vctBar;
public:
	CWnd*		GetMainView();
	CWnd*		GetBarView(int nNO);
	BOOL		SetView(CRuntimeClass* pViewClass);
	int			AddBarView(CRuntimeClass* pViewClass, LPCSTR lpTitle = NULL);
	BOOL		DeleteBarView(int nNO);
	BOOL		ExpendBar(int nNO, BOOL bExpend);
	BOOL		SetBarHeight(int nNO, int nHeight, int nMaxHeight = -1, LPCSTR lpTitle = NULL);

	void		RecalcLayout();
	BOOL		GetBarViewRect(int nNO, CRect& rect);
	
	virtual void	DrawBarTitle(CDC* pDC, CRect& rect, int nNO);
	virtual int		HitTest(CPoint& pt);
	virtual int		HitSpliter(CPoint& pt);

	virtual void	StartTracking(int nNO);
	virtual void	StopTracking(BOOL bActive);
	virtual void	OnInvertTracker(CRect rtTracker);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


