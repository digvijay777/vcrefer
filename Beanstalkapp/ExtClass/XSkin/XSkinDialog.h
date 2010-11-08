#pragma once


class CXSkinDialog : public CDialog
{
	DECLARE_DYNAMIC(CXSkinDialog)

public:
	CXSkinDialog(UINT nIDTemplate, CWnd* pParent = NULL);   
	virtual ~CXSkinDialog();

	DECLARE_MESSAGE_MAP()
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	CBrush		m_brushDlgBk;
	CPen		m_penEdgeInner;
	CPen		m_penEdgeOuter;
	BOOL		m_bNcLButtonDown;
private:
	BOOL	DrawFrame();
	void	DrawTitleBar(CDC* pDC, DWORD nFlag = 0x0);
private:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
};
