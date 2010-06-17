#pragma once


// CExtTreeCtrl

class CExtTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CExtTreeCtrl)

public:
	CExtTreeCtrl();
	virtual ~CExtTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
};


