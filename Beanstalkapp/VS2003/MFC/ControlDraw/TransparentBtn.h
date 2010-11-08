#pragma once


// CTransparentBtn

class CTransparentBtn : public CButton
{
	DECLARE_DYNAMIC(CTransparentBtn)

public:
	CTransparentBtn();
	virtual ~CTransparentBtn();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


