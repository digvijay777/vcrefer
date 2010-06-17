#pragma once


class CRadioButton : public CButton
{
public:
	CRadioButton();
	virtual ~CRadioButton();

	DECLARE_MESSAGE_MAP()
public:

protected:
	virtual void PreSubclassWindow();
	void	DrawBk(CDC* pDC);
	BOOL	DrawBitmap(CDC* pDC, CBitmap* pBmp, CRect& rtDC, CPoint& ptBmp);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	CBitmap			m_RadioBitmap;
	BITMAP			m_RadioBmpInfo;
public:
	BOOL LoadBitmap(UINT nID);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};