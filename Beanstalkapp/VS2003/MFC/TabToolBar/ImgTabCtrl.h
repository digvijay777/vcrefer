#pragma once


// CImgTabCtrl

class CImgTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CImgTabCtrl)

public:
	CImgTabCtrl();
	virtual ~CImgTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

	void		InitTabCtrl();
private:
	CImageList		m_ImgList;
	CImageList		m_ImgIcon;
	IMAGEINFO		m_ImgInfoItem;
	IMAGEINFO		m_ImgInfoIcon;
	COLORREF		m_colText;
	COLORREF		m_colSelText;
	BOOL			m_bTrackMouseEvent;
	int				m_nMouseHover;
	BOOL			m_bHasMouseHoverEvent;
	int				m_nIconLeftPos;
	int				m_nIconRightPos;
	CFont			m_FontSel;
public:
	BOOL		LoadImages(UINT uImgItem, int nState = 2, UINT uImgIcon = -1, COLORREF colBk = RGB(0xFF, 0x0, 0xFF));
	BOOL		LoadImages(LPSTR lpImgItem, int nState = 2, LPSTR lpImgIcon = NULL, COLORREF colBk = RGB(0xFF, 0x0, 0xFF));
	BOOL		SetBitmaps(CBitmap* pBmpItem, int nState = 2, CBitmap* pBmpIcon = NULL, COLORREF colBk = RGB(0xFF, 0x0, 0xFF));
	void		SetTabVertical(BOOL bVertical = FALSE);
	BOOL		SetIconPos(int nLeft = 8, int nRight = 5);
	void		SetTextColor(COLORREF colText, COLORREF colSelText);

private:
	void		SetMouseHoverItem(int nItem);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void DrawItem_Bk(CDC* pDC, CRect& rect, int nState);
	virtual void DrawItem_Text(CDC* pDC, int nItem, LPCSTR lpText, CRect& rect, int nState);
	virtual void DrawItem_Icon(CDC* pDC, int nImage, CRect& rect);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
};


