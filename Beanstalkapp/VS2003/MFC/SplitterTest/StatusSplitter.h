#pragma once


// CStatusSplitter

class CStatusHSplitter : public CSplitterWnd
{
	DECLARE_DYNAMIC(CStatusHSplitter)

public:
	CStatusHSplitter();
	virtual ~CStatusHSplitter();

private:
	BOOL	m_bSplitter;

protected:
	void	InitRect(int nRows);

public:
	BOOL	InitSplitter(CWnd* pParent, CCreateContext* pContext, int nRows, ...);


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInvertTracker(const CRect& rect);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);
	virtual void TrackRowSize(int y, int row);
	virtual void RecalcLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};


